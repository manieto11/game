#include "player.h"
#include "settings.h"
#include "game_utils.h"

Entity *PlayerEntity;
float elapsedCoyoteTime, elapsedJumpTime;
bool doubleJump;

bool IsGrounded(Platform** platforms, int platformCount)
{
    for (int i = 0; i < platformCount; ++i)
    {
        Vector2 platformPosition = b2Body_GetPosition(platforms[i]->body), offset = 0.5f * platforms[i]->size, topRight = platformPosition + offset, bottomLeft = platformPosition - offset;
        float playerOffsetX = 0.45f * PlayerEntity->size.x, playerOffsetY = 0.5f * PlayerEntity->size.y;
        Vector2 playerPosition = b2Body_GetPosition(PlayerEntity->body), offsetCenter = {playerPosition.x, playerPosition.y - playerOffsetY}, offsetLeft = {playerPosition.x - playerOffsetX, playerPosition.y - playerOffsetY}, offsetRight = {playerPosition.x + playerOffsetX, playerPosition.y - playerOffsetY};

        if (PointInsideBox(topRight, bottomLeft, offsetCenter) || 
            PointInsideBox(topRight, bottomLeft, offsetLeft) ||
            PointInsideBox(topRight, bottomLeft, offsetRight))
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

void UpdatePlayer(Platform** platforms, int platformCount)
{
    if (PlayerEntity == nullptr)
        return;

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

    SetRange(movingX, -PLAYER_SPEED, PLAYER_SPEED);

    b2Body_SetLinearVelocity(PlayerEntity->body, {movingX, velocity.y});
}
