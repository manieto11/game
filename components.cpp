#include "components.h"
#include "settings.h"
#include "textures.h"

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
    dest.x = ((int)transform->position.x + transform->scale.y / 2) * PIXELS_PER_UNIT;
    dest.y = (-(int)(transform->position.y + transform->scale.y)) * PIXELS_PER_UNIT;
    dest.width = ((int)transform->scale.x) * PIXELS_PER_UNIT;
    dest.height = ((int)transform->scale.y) * PIXELS_PER_UNIT;

    if (texture == nullptr or !IsTextureValid(*texture)) 
        TraceLog(LOG_WARNING, "Texture error!");
    DrawTexturePro(*texture, spriteRec, dest, {(transform->scale.x) * PIXELS_PER_UNIT, (-transform->scale.y) * PIXELS_PER_UNIT / 2}, transform->rotation, color);
}

Collider::Collider(float width, float height) : isRound(false), width(width), height(height) {}

Collider::Collider(bool isRound, float diameter) : isRound(isRound), width(diameter), height(diameter) {}

void RigidBody::ApplyGravity()
{
    if (grounded or gravity == 0.0f) return;

    velocity = {velocity.x, velocity.y - gravity * FIXED_DELTA_TIME};
}

RigidBody::RigidBody(Transform2D *transform, Collider *collider, float gravity) : transform(transform), collider(collider), gravity(gravity), velocity({0.0f, 0.0f}) {}

void RigidBody::ApplyPhysics()
{
    ApplyGravity();
    transform->position = {transform->position.x + velocity.x * FIXED_DELTA_TIME, transform->position.y + velocity.y * FIXED_DELTA_TIME};
}
