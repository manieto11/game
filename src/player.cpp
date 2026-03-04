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
        Vector2 offset = 0.5f * platforms[i]->body.width, topRight = platforms[i]->body.position + offset, bottomLeft = platforms[i]->body.position - offset;
        float playerOffsetX = 0.45f * PlayerEntity->body.width.x, playerOffsetY = 0.5f * PlayerEntity->body.width.y;
        Vector2 offsetCenter = {PlayerEntity->body.position.x, PlayerEntity->body.position.y - playerOffsetY}, offsetLeft = {PlayerEntity->body.position.x - playerOffsetX, PlayerEntity->body.position.y - playerOffsetY}, offsetRight = {PlayerEntity->body.position.x + playerOffsetX, PlayerEntity->body.position.y - playerOffsetY};

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

    PlayerEntity->body.velocity.y = 7.0f;
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
        movingX = grounded ? -PLAYER_SPEED : PlayerEntity->body.velocity.x - PLAYER_AIR_ACCELERATION;
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        movingX = grounded ? PLAYER_SPEED : PlayerEntity->body.velocity.x + PLAYER_AIR_ACCELERATION;
    else
        if (grounded || PlayerEntity->body.velocity.x == 0.0f) 
            movingX = 0.0f;
        else
            movingX = PlayerEntity->body.velocity.x > 0.0f ? (PlayerEntity->body.velocity.x - PLAYER_AIR_ACCELERATION / 2.0f) : (PlayerEntity->body.velocity.x + PLAYER_AIR_ACCELERATION / 2.0f);

    SetRange(movingX, -PLAYER_SPEED, PLAYER_SPEED);

    PlayerEntity->body.velocity.x = movingX;
}
