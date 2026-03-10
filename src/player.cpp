#include "player.h"
#include "materials.h"
#include "meshes.h"
#include "raymath.h"
#include "settings.h"

Entity *PlayerEntity;
float inputX, elapsedCoyoteTime, lastJumpTime, step = 0.0f, bodyRotation;
bool doubleJump, unlockedDoubleJump = false, grounded;
b2Vec2 playerPosition, playerVelocity, bodyPos, leftLegPos, rightLegPos;

void UpdatePlayerAnimation()
{
    if (grounded && fabs(playerVelocity.x) > 0.1f)
    {
        step += GetFrameTime() * speedMultiplier * 3.0f;
        if (step > 1.0f) 
            step -= 1.0f;
    }
    
    bodyRotation = 10.0f * DEG2RAD * (playerVelocity.x / -(PLAYER_SPEED));
    
    bodyPos = {playerPosition.x, playerPosition.y + 0.05f * sinf(2.0f * step * PI)};
}

bool IsGrounded(Platform** platforms, int platformCount)
{
    for (int i = 0; i < platformCount; ++i)
    {
        b2Vec2 platformPosition = b2Body_GetPosition(platforms[i]->body), offset = 0.5f * platforms[i]->size;
        Rectangle platformRect = {platformPosition.x - offset.x, platformPosition.y - offset.y, platforms[i]->size.x, platforms[i]->size.y};
        float playerOffsetX = 0.45f * PlayerEntity->size.x, playerOffsetY = 0.5f * PlayerEntity->size.y;
        Vector2 offsetCenter = {playerPosition.x, playerPosition.y - playerOffsetY}, offsetLeft = {playerPosition.x - playerOffsetX, playerPosition.y - playerOffsetY}, offsetRight = {playerPosition.x + playerOffsetX, playerPosition.y - playerOffsetY};

        if (CheckCollisionPointRec(offsetCenter, platformRect) || 
            CheckCollisionPointRec(offsetLeft, platformRect) ||
            CheckCollisionPointRec(offsetRight, platformRect))
                return true;
    }

    return false;
}

void Jump()
{
    lastJumpTime = GetTime();

    float jumpingVelocity = sqrtf(2.0f * PLAYER_JUMP_HEIGHT * GAME_GRAVITY);

    // TraceLog(LOG_INFO, "Set jumping velocity to %.1f", jumpingVelocity);

    playerVelocity.y = jumpingVelocity;
    // TraceLog(LOG_INFO, "Jump with velocity {%.1f, %.1f}!", b2Body_GetLinearVelocity(PlayerEntity->body).x, b2Body_GetLinearVelocity(PlayerEntity->body).y);
}

void DrawPlayer()
{
    Matrix sizeMat = MatrixScale(PlayerEntity->size.x, PlayerEntity->size.x, 1.0f), 
        bodyTransform = MatrixRotateZ(bodyRotation) * MatrixTranslate(PIXELS_PER_UNIT * bodyPos.x, - PIXELS_PER_UNIT * bodyPos.y, 0.0f) * sizeMat,
        leftLegTransform  = MatrixTranslate(PIXELS_PER_UNIT * leftLegPos.x, - PIXELS_PER_UNIT * leftLegPos.y, 0.0f) * sizeMat,
        rightLegTransform = MatrixTranslate(PIXELS_PER_UNIT * rightLegPos.x, - PIXELS_PER_UNIT * rightLegPos.y, 0.0f) * sizeMat;

    DrawMesh(PlayerBodyMesh, PlayerMaterial, bodyTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, leftLegTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, rightLegTransform);
}

void UpdatePlayer(Platform **platforms, int platformCount)
{
    if (PlayerEntity == nullptr || !b2Body_IsValid(PlayerEntity->body))
        return;

    playerPosition = b2Body_GetPosition(PlayerEntity->body);

    grounded = IsGrounded(platforms, platformCount);
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
        elapsedCoyoteTime += GetFrameTime();
    }

    //float movingX = grounded ? 0.0f : playerVelocity.x;

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
    {
        float axisValue = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_LEFT_X);
        
#if DEBUG
        const char* gamepadName = GetGamepadName(PLAYER_GAMEPAD);
        TraceLog(LOG_INFO, "Gamepad: %s, Axis: %.3f", gamepadName ? gamepadName : "Unknown", axisValue);
#endif
        
        if (fabs(axisValue) > 0.1f)
            inputX = axisValue;
    }
    else if ((IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && !(IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX -= INPUT_ACCELERATION;
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX += INPUT_ACCELERATION;
    else
        if (fabs(inputX) < 0.1f)
            inputX = 0.0f;
        else
            inputX += inputX > 0.0f ? -INPUT_ACCELERATION : INPUT_ACCELERATION;

    inputX = Clamp(inputX, - 1.0f, 1.0f);

    if ((GetTime() - lastJumpTime > PLAYER_JUMP_DELAY) && (IsKeyDown(KEY_SPACE) || (IsGamepadAvailable(PLAYER_GAMEPAD) && IsGamepadButtonDown(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) && (grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || (unlockedDoubleJump && doubleJump)))
    {
        if (!(grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME)) 
            doubleJump = false;
        Jump();
    }

    b2Body_SetLinearVelocity(PlayerEntity->body, {inputX * PLAYER_SPEED, playerVelocity.y});
}
