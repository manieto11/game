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

    if (IsKeyPressed(KEY_SPACE))
        Jump();

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
        PlayerEntity->body.velocity.x = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_RIGHT_X);
    else if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
        PlayerEntity->body.velocity.x = -PLAYER_SPEED;
    else if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        PlayerEntity->body.velocity.x = PLAYER_SPEED;
    else
        PlayerEntity->body.velocity.x = 0.0f;
}
