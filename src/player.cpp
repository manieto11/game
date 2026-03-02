#include "player.h"
#include "settings.h"

Entity *PlayerEntity;

void Jump()
{
    PlayerEntity->body.velocity.y = 10.0f;
}

void UpdatePlayer()
{
    if (PlayerEntity == nullptr)
        return;

    if (IsKeyPressed(KEY_SPACE) || (IsGamepadAvailable(PLAYER_GAMEPAD) && IsGamepadButtonPressed(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)))
        Jump();

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
    {
        float axisValue = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_LEFT_X);
        
        // Debug output
        const char* gamepadName = GetGamepadName(PLAYER_GAMEPAD);
        TraceLog(LOG_INFO, "Gamepad: %s, Axis: %.3f", gamepadName ? gamepadName : "Unknown", axisValue);
        
        // Apply dead zone
        if (fabs(axisValue) > 0.1f)
            PlayerEntity->body.velocity.x = axisValue * PLAYER_SPEED;
        else
            PlayerEntity->body.velocity.x = 0.0f;
    }
    else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
        PlayerEntity->body.velocity.x = -PLAYER_SPEED;
    else if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        PlayerEntity->body.velocity.x = PLAYER_SPEED;
    else
        PlayerEntity->body.velocity.x = 0.0f;
}
