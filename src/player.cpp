#include "player.h"
#include "materials.h"
#include "meshes.h"
#include "raymath.h"
#include "settings.h"

Entity *PlayerEntity;
float elapsedCoyoteTime, elapsedJumpTime;
bool doubleJump;
Vector2 playerPosition;

bool IsGrounded(Platform** platforms, int platformCount)
{
    for (int i = 0; i < platformCount; ++i)
    {
        Vector2 platformPosition = b2Body_GetPosition(platforms[i]->body), offset = 0.5f * platforms[i]->size;
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
    if (elapsedJumpTime != 0.0f)
        elapsedJumpTime = 0.0f;

    b2Body_SetLinearVelocity(PlayerEntity->body, {b2Body_GetLinearVelocity(PlayerEntity->body).x, 7.0f});
}

void DrawPlayer()
{
    Matrix bodyTransform = MatrixScale(PIXELS_PER_UNIT, PIXELS_PER_UNIT, 1.0f) * MatrixTranslate(PIXELS_PER_UNIT * playerPosition.x, - PIXELS_PER_UNIT * playerPosition.y, 0.0f); //MatrixTranslate(playerPosition.x, playerPosition.y, 0.0f) * MatrixRotateZ(1.57f);
    DrawMesh(PlayerBodyMesh, PlayerMaterial, bodyTransform);
}

void UpdatePlayer(Platform **platforms, int platformCount)
{
    if (PlayerEntity == nullptr || !b2Body_IsValid(PlayerEntity->body))
        return;

    playerPosition = b2Body_GetPosition(PlayerEntity->body);

    bool grounded = IsGrounded(platforms, platformCount);

    if (grounded) 
    {
        if (elapsedCoyoteTime != 0.0f)
            elapsedCoyoteTime = 0.0f;
            doubleJump = true;
    } 
    else 
    {
        elapsedCoyoteTime += GetFrameTime();
        elapsedJumpTime += GetFrameTime();
    }

    if ((IsKeyDown(KEY_SPACE) || (IsGamepadAvailable(PLAYER_GAMEPAD) && IsGamepadButtonDown(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) && (grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || (doubleJump && elapsedJumpTime > PLAYER_DOUBLE_JUMP_DELAY)))
    {
        if (!(grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME)) 
            doubleJump = false;
        Jump();
    }

    Vector2 velocity = b2Body_GetLinearVelocity(PlayerEntity->body);

    float movingX;

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
    {
        float axisValue = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_LEFT_X);
        
        // Debug output
        const char* gamepadName = GetGamepadName(PLAYER_GAMEPAD);
        TraceLog(LOG_INFO, "Gamepad: %s, Axis: %.3f", gamepadName ? gamepadName : "Unknown", axisValue);
        
        // Apply dead zone
        if (fabs(axisValue) > 0.1f)
            movingX = axisValue * PLAYER_SPEED;
    }
    else if ((IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && !(IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        movingX = grounded ? -PLAYER_SPEED : velocity.x - PLAYER_AIR_ACCELERATION;
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        movingX = grounded ? PLAYER_SPEED : velocity.x + PLAYER_AIR_ACCELERATION;
    else
        if (grounded || velocity.x == 0.0f) 
            movingX = 0.0f;
        else
            movingX = velocity.x > 0.0f ? (velocity.x - PLAYER_AIR_ACCELERATION / 2.0f) : (velocity.x + PLAYER_AIR_ACCELERATION / 2.0f);

    Wrap(movingX, -PLAYER_SPEED, PLAYER_SPEED);

    b2Body_SetLinearVelocity(PlayerEntity->body, {movingX, velocity.y});
}
