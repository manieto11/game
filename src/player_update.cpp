#include "player_internal.h"
#include "game.h"

#include <cmath>

namespace
{
    constexpr float kInputDeadzone = 0.1f;
    constexpr float kStrideSegment = 0.25f;

    bool CheckRectCollisionAgainstGroundGrid(const Rectangle& probeRect)
    {
        for (auto& grid : Grids)
        {
            if (grid == nullptr || !grid->hasCollision)
                continue;

            const float cellWidth = grid->cellSize.x;
            const float cellHeight = grid->cellSize.y;

            int minX = static_cast<int>(floorf(probeRect.x / cellWidth));
            int maxX = static_cast<int>(floorf((probeRect.x + probeRect.width) / cellWidth));
            int minY = static_cast<int>(floorf(probeRect.y / cellHeight));
            int maxY = static_cast<int>(floorf((probeRect.y + probeRect.height) / cellHeight));

            minX = Clamp(minX, 0, grid->gridWidth - 1);
            maxX = Clamp(maxX, 0, grid->gridWidth - 1);
            minY = Clamp(minY, 0, grid->gridHeight - 1);
            maxY = Clamp(maxY, 0, grid->gridHeight - 1);

            for (int y = minY; y <= maxY; ++y)
            {
                for (int x = minX; x <= maxX; ++x)
                {
                    const GridCell* cell = grid->GetCell(x, y);
                    if (cell == nullptr || cell->textureID == 255)
                        continue;

                    const Rectangle cellRect = {x * cellWidth, y * cellHeight, cellWidth, cellHeight};
                    if (CheckCollisionRecs(cellRect, probeRect))
                        return true;
                }
            }
        }

        return false;
    }

    float LegStrideY(float cycle, bool invert)
    {
        float y = 0.0f;
        if (cycle < kStrideSegment)
            y = 0.0f;
        else if (cycle < 2.0f * kStrideSegment)
            y = Normalize(cycle, kStrideSegment, 2.0f * kStrideSegment);
        else if (cycle < 3.0f * kStrideSegment)
            y = 1.0f;
        else
            y = 1.0f - Normalize(cycle, 3.0f * kStrideSegment, 1.0f);

        return invert ? 1.0f - y : y;
    }
}

void UpdatePlayerAnimation()
{
    if (grounded && fabs(playerVelocity.x) > kInputDeadzone)
    {
        step += GetFrameTime() * speedMultiplier * 3.0f;
        while (step >= 1.0f)
            step -= 1.0f;
    }

    const float playerSpeed = GetPlayerSpeed();
    bodyRotation = playerSpeed != 0.0f ? -10.0f * (playerVelocity.x / playerSpeed) : 0.0f;

    bodyPos = {playerPosition.x, playerPosition.y + 0.05f * cosf(2.0f * PI * step)};

    const bool moving = fabs(playerVelocity.x) > kInputDeadzone;
    const float legYOffset = moving ? 0.15f : 0.0f;
    leftLegPos = {bodyPos.x - 0.6f, bodyPos.y - 0.5f + legYOffset * LegStrideY(step, false)};
    rightLegPos = {bodyPos.x + 0.6f, bodyPos.y - 0.5f + legYOffset * LegStrideY(step, true)};
}

bool WallClimb()
{
    const Rectangle leftRect = WallProbeRect(playerPosition, false);
    const Rectangle rightRect = WallProbeRect(playerPosition, true);

    if (CheckRectCollisionAgainstGroundGrid(leftRect))
    {
        bodyUp = kWallUpLeft;
        return true;
    }

    if (CheckRectCollisionAgainstGroundGrid(rightRect))
    {
        bodyUp = kWallUpRight;
        return true;
    }

    bodyUp = kFloorUp;
    return false;
}

bool IsGrounded()
{
    const Rectangle groundedRect = GroundProbeRect(playerPosition);
    return CheckRectCollisionAgainstGroundGrid(groundedRect);
}

void Jump()
{
    lastJumpTime = GetTime();

    const float jumpingVelocity = sqrtf(2.0f * GetPlayerJumpHeight() * GAME_GRAVITY);

    playerVelocity += jumpingVelocity * bodyUp;
}

void UpdatePlayer()
{
    if (PlayerEntity == nullptr || !b2Body_IsValid(PlayerEntity->body))
        return;

    playerPosition = b2Body_GetPosition(PlayerEntity->body);

    const float frameTime = GetFrameTime();
    const float currentTime = GetTime();
    const bool gamepadAvailable = IsGamepadAvailable(PLAYER_GAMEPAD);

    grounded = IsGrounded();

    if (unlockedWallClimbing)
        wallClimbing = WallClimb();
    else if (wallClimbing)
        wallClimbing = false;

    playerVelocity = b2Body_GetLinearVelocity(PlayerEntity->body);

    UpdatePlayerAnimation();

    if (grounded)
    {
        if (elapsedCoyoteTime != 0.0f)
            elapsedCoyoteTime = 0.0f;
        doubleJump = true;
    }
    else
    {
        elapsedCoyoteTime += frameTime;
    }

    if (gamepadAvailable)
    {
        float axisValue = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_LEFT_X);

#if DEBUG
        const char* gamepadName = GetGamepadName(PLAYER_GAMEPAD);
        TraceLog(LOG_INFO, "Gamepad: %s, Axis: %.3f", gamepadName ? gamepadName : "Unknown", axisValue);
#endif

        if (fabs(axisValue) > kInputDeadzone)
            inputX = axisValue;
    }
    else if ((IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && !(IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX -= GetInputAcceleration();
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX += GetInputAcceleration();
    else
    {
        if (fabs(inputX) < kInputDeadzone)
            inputX = 0.0f;
        else
            inputX += inputX > 0.0f ? -GetInputAcceleration() : GetInputAcceleration();
    }

    inputX = Clamp(inputX, -1.0f, 1.0f);

    if ((inputX > 0.0f && lookingRight) || (inputX < 0.0f && !lookingRight))
        lookingRight = !lookingRight;

    const float playerSpeed = GetPlayerSpeed();

    const bool jumpPressed = IsKeyDown(KEY_SPACE) || (gamepadAvailable && IsGamepadButtonDown(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
    const bool canJump = grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || (unlockedDoubleJump && doubleJump) || (unlockedWallClimbing && wallClimbing);
    if ((currentTime - lastJumpTime > PLAYER_JUMP_DELAY) && jumpPressed && canJump)
    {
        if (!(grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || wallClimbing))
            doubleJump = false;
        Jump();
    }

    if (bodyUp == kWallUpLeft && !(grounded && inputX > 0.0f))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {playerVelocity.x - frameTime, -inputX * playerSpeed});
    }
    else if (bodyUp == kWallUpRight && !(grounded && inputX < 0.0f))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {playerVelocity.x + frameTime, inputX * playerSpeed});
    }
    else if (grounded && (currentTime - lastJumpTime > PLAYER_JUMP_DELAY))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {inputX * playerSpeed, 0.0f});
    }
    else
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, playerVelocity);
    }
}