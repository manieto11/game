#include "player.h"
#include "materials.h"
#include "meshes.h"
#include "raymath.h"
#include "settings.h"

Entity *PlayerEntity;
float inputX, elapsedCoyoteTime, lastJumpTime, step = 0.0f, bodyRotation, leftLegRotation, rightLegRotation;
bool doubleJump, unlockedDoubleJump = false, grounded, wallClimbing = false, unlockedWallClimbing = true;
b2Vec2 playerPosition, playerVelocity, bodyPos, leftLegPos, rightLegPos, bodyUp = {0.0f, 1.0f};

namespace
{
    constexpr float kInputDeadzone = 0.1f;
    constexpr float kStrideSegment = 0.25f;

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

    Rectangle PlatformRect(const Platform* platform)
    {
        const b2Vec2 platformPosition = b2Body_GetPosition(platform->body);
        const b2Vec2 offset = 0.5f * platform->size;
        return {platformPosition.x - offset.x, platformPosition.y - offset.y, platform->size.x, platform->size.y};
    }

    Rectangle GroundProbeRect(const b2Vec2& position)
    {
        const b2Vec2 offset = {0.5f * PlayerEntity->size.x, PlayerEntity->size.y / 2.0f + 0.05f};
        return {position.x - offset.x, position.y - offset.y, PlayerEntity->size.x, 0.1f};
    }

    Rectangle WallProbeRect(const b2Vec2& position, bool rightSide)
    {
        const b2Vec2 offset = {0.5f * PlayerEntity->size.x + 0.05f, PlayerEntity->size.y * 0.95f / 2.0f};
        const float x = rightSide ? position.x + offset.x : position.x - offset.x;
        return {x, position.y - offset.y, 0.1f, PlayerEntity->size.y * 0.95f};
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
    
    bodyRotation = 10.0f * DEG2RAD * (playerVelocity.x / -(PLAYER_SPEED));
    
    bodyPos = {playerPosition.x, playerPosition.y + 0.05f * cosf(2.0f * PI * step)};

    const bool moving = fabs(playerVelocity.x) > kInputDeadzone;
    const float legYOffset = moving ? 0.15f : 0.0f;
    leftLegPos = {bodyPos.x - 0.6f, bodyPos.y - 0.5f + legYOffset * LegStrideY(step, false)};
    rightLegPos = {bodyPos.x + 0.6f, bodyPos.y - 0.5f + legYOffset * LegStrideY(step, true)};
}

bool WallClimb(Platform** platforms, int platformCount)
{
    const Rectangle leftRect = WallProbeRect(playerPosition, false);
    const Rectangle rightRect = WallProbeRect(playerPosition, true);

    for (int i = 0; i < platformCount; ++i)
    {
        const Rectangle platformRect = PlatformRect(platforms[i]);

        if (CheckCollisionRecs(platformRect, leftRect))
        {
            bodyUp = {1.0f, 0.0f};
            return true;
        }
        else if (CheckCollisionRecs(platformRect, rightRect))
        {
            bodyUp = {-1.0f, 0.0f};
            return true;
        }
    }

    bodyUp = {0.0f, 1.0f};
    return false;
}

bool IsGrounded(Platform** platforms, int platformCount)
{
    const Rectangle groundedRect = GroundProbeRect(playerPosition);

    for (int i = 0; i < platformCount; ++i)
    {
        const Rectangle platformRect = PlatformRect(platforms[i]);

        if (CheckCollisionRecs(platformRect, groundedRect))
            return true;
    }

    return false;
}

void Jump()
{
    lastJumpTime = GetTime();

    float jumpingVelocity = sqrtf(2.0f * (PLAYER_JUMP_HEIGHT) * GAME_GRAVITY);

    playerVelocity += jumpingVelocity * bodyUp;
}

void DrawPlayer()
{
    Matrix sizeMat = MatrixScale(PlayerEntity->size.x, PlayerEntity->size.x, 1.0f), 
        bodyTransform = MatrixRotateZ(bodyRotation) * MatrixTranslate(PIXELS_PER_UNIT * bodyPos.x, - PIXELS_PER_UNIT * bodyPos.y, 0.0f) * sizeMat,
        leftLegTransform  = MatrixRotateZ(leftLegRotation) * MatrixTranslate(PIXELS_PER_UNIT * leftLegPos.x, - PIXELS_PER_UNIT * leftLegPos.y, 0.0f) * sizeMat,
        rightLegTransform = MatrixRotateZ(rightLegRotation) * MatrixTranslate(PIXELS_PER_UNIT * rightLegPos.x, - PIXELS_PER_UNIT * rightLegPos.y, 0.0f) * sizeMat;

    DrawMesh(PlayerBodyMesh, PlayerMaterial, bodyTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, leftLegTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, rightLegTransform);
}

void DrawPlayerDebug()
{
    const Rectangle groundedRect = GroundProbeRect(playerPosition);
    DrawRectangleLines(groundedRect.x * PIXELS_PER_UNIT, -groundedRect.y * PIXELS_PER_UNIT, groundedRect.width * PIXELS_PER_UNIT, groundedRect.height * PIXELS_PER_UNIT, grounded ? LIME : RED);

    const Rectangle leftRect = WallProbeRect(playerPosition, false);
    const Rectangle rightRect = WallProbeRect(playerPosition, true);
    DrawRectangleLines(leftRect.x * PIXELS_PER_UNIT, (-leftRect.y - 0.95f * PlayerEntity->size.y) * PIXELS_PER_UNIT, leftRect.width * PIXELS_PER_UNIT, leftRect.height * PIXELS_PER_UNIT, bodyUp == b2Vec2({1.0f, 0.0f}) ? LIME : RED);
    DrawRectangleLines(rightRect.x * PIXELS_PER_UNIT, (-rightRect.y - 0.95f * PlayerEntity->size.y) * PIXELS_PER_UNIT, rightRect.width * PIXELS_PER_UNIT, rightRect.height * PIXELS_PER_UNIT, bodyUp == b2Vec2({-1.0f, 0.0f}) ? LIME : RED);
}

void UpdatePlayer(Platform **platforms, int platformCount)
{
    if (PlayerEntity == nullptr || !b2Body_IsValid(PlayerEntity->body))
        return;

    playerPosition = b2Body_GetPosition(PlayerEntity->body);

    const float frameTime = GetFrameTime();
    const float currentTime = GetTime();
    const bool gamepadAvailable = IsGamepadAvailable(PLAYER_GAMEPAD);

    grounded = IsGrounded(platforms, platformCount);

    if (unlockedWallClimbing)
        wallClimbing = WallClimb(platforms, platformCount);
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
        inputX -= INPUT_ACCELERATION;
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX += INPUT_ACCELERATION;
    else
    {
        if (fabs(inputX) < kInputDeadzone)
            inputX = 0.0f;
        else
            inputX += inputX > 0.0f ? -INPUT_ACCELERATION : INPUT_ACCELERATION;
    }

    inputX = Clamp(inputX, - 1.0f, 1.0f);

    const bool jumpPressed = IsKeyDown(KEY_SPACE) || (gamepadAvailable && IsGamepadButtonDown(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
    const bool canJump = grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || (unlockedDoubleJump && doubleJump) || (unlockedWallClimbing && wallClimbing);
    if ((currentTime - lastJumpTime > PLAYER_JUMP_DELAY) && jumpPressed && canJump)
    {
        if (!(grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || wallClimbing))
            doubleJump = false;
        Jump();
    }

    if (bodyUp == b2Vec2({1.0f, 0.0f}) && !(grounded && inputX > 0.0f))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {playerVelocity.x - frameTime, -inputX * PLAYER_SPEED});
    }
    else if (bodyUp == b2Vec2({-1.0f, 0.0f}) && !(grounded && inputX < 0.0f))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {playerVelocity.x + frameTime, inputX * PLAYER_SPEED});
    }
    else if (grounded && (currentTime - lastJumpTime > PLAYER_JUMP_DELAY))
    {
        b2Body_SetLinearVelocity(PlayerEntity->body, {inputX * PLAYER_SPEED, 0.0f});
    }
    else b2Body_SetLinearVelocity(PlayerEntity->body, playerVelocity);
}
