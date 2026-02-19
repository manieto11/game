#include "components.h"
#include "settings.h"
#include "textures.h"

Transform2D::Transform2D()
{
    position = {0.0f, 0.0f};
    scale = {1.0f, 1.0f};
    rotation = 0;
}

SpriteRenderer::SpriteRenderer(Transform2D *transform) : transform(transform)
{
    texture = &DefaultTexture;
    spriteRec = {0, 0, PIXELS_PER_UNIT, PIXELS_PER_UNIT};
    color = WHITE;
}

void SpriteRenderer::Draw()
{
    Rectangle dest;
    dest.x = (transform->position.x + transform->scale.y / 2) * PIXELS_PER_UNIT;
    dest.y = (-(transform->position.y + transform->scale.y)) * PIXELS_PER_UNIT;
    dest.width = (transform->scale.x) * PIXELS_PER_UNIT;
    dest.height = (transform->scale.y) * PIXELS_PER_UNIT;

    if (texture == nullptr or !IsTextureValid(*texture))
        TraceLog(LOG_WARNING, "Texture error!");
    DrawTexturePro(*texture, spriteRec, dest, {(transform->scale.x) * PIXELS_PER_UNIT, (-transform->scale.y) * PIXELS_PER_UNIT / 2}, transform->rotation, color);
}

Collider::Collider(float width, float height) : isRound(false), width(width), height(height) {}

Collider::Collider(bool isRound, float diameter) : isRound(isRound), width(diameter), height(diameter) {}

void RigidBody::ApplyGravity()
{
    if (grounded or gravity == 0.0f)
        return;

    velocity.y -= gravity * FIXED_DELTA_TIME;
}

RigidBody::RigidBody(Transform2D *transform, Collider *collider, float gravity) : transform(transform), collider(collider), gravity(gravity), velocity({0.0f, 0.0f}), grounded(false) {}

void RigidBody::ApplyPhysics()
{
    ApplyGravity();
    transform->position.x += velocity.x * FIXED_DELTA_TIME;
    transform->position.y += velocity.y * FIXED_DELTA_TIME;
}

PlayerController::PlayerController(RigidBody *rigidBody, float moveSpeed) : rigidBody(rigidBody), enabled(true), moveSpeed(moveSpeed) {}

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
