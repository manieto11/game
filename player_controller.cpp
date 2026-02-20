#include "player_controller.h"
#include "settings.h"

PlayerController::PlayerController(RigidBody *rigidBody) : rigidBody(rigidBody), enabled(true), moveSpeed(PLAYER_SPEED) {}

void PlayerController::SetEnabled(bool value)
{
    enabled = value;
}

void PlayerController::Update()
{
    if (!enabled || rigidBody == nullptr)
        return;

    float horizontalMovement = 0.0f;

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
        horizontalMovement = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_RIGHT_X);

    if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
        horizontalMovement = 1.0f;
    else if (!IsKeyDown(KEY_D) && IsKeyDown(KEY_A))
        horizontalMovement = -1.0f;

    rigidBody->velocity.x = horizontalMovement;
}