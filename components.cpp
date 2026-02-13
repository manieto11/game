#include "components.h"
#include "settings.h"
#include "textures.h"
#include <cmath>

Transform2D::Transform2D()
{
    position = {0, 0};
    scale = {1, 1};
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

RigidBody::RigidBody(Transform2D *transform, Collider *collider, float gravity) : transform(transform), collider(collider), gravity(gravity), velocity({0.0f, 0.0f}) {}

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

    Vector2 input = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
        input.y += 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        input.y -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        input.x += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        input.x -= 1.0f;

    if (input.x != 0.0f || input.y != 0.0f)
    {
        float length = sqrtf((input.x * input.x) + (input.y * input.y));
        input = {input.x / length, input.y / length};
    }

    Vector2 velocity = {input.x * moveSpeed, input.y * moveSpeed};

    rigidBody->velocity = velocity;
}
