#include "rigid_body.h"
#include "settings.h"

void RigidBody::ApplyGravity()
{
    if (grounded or gravity == 0.0f)
        return;

    velocity.y -= gravity * FIXED_DELTA_TIME;
}

RigidBody::RigidBody(Transform2D *transform, Collider *collider, float gravity) : transform(transform), collider(collider), gravity(gravity), velocity({0.0f, 0.0f}), grounded(false) {}

RigidBody::RigidBody(Transform2D * transform, Collider * collider) : transform(transform), collider(collider), gravity(DEFAULT_GRAVITY), velocity({0.0f, 0.0f}), grounded(false) {}

void RigidBody::ApplyPhysics()
{
    ApplyGravity();
    transform->position.x += velocity.x * FIXED_DELTA_TIME;
    transform->position.y += velocity.y * FIXED_DELTA_TIME;
}