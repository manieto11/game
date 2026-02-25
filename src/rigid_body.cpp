#include "rigid_body.h"
#include "game.h"
#include "settings.h"

void RigidBody::ApplyGravity()
{
    if (grounded)
    {
        if (velocity.y < 0.0f) velocity.y = 0.0f;
        return;
    }

    if (gravity != 0.0f) velocity.y -= gravity * FIXED_DELTA_TIME;
}

void RigidBody::CheckCollisions()
{
    if (GameColliders == nullptr || ColliderCount <= 0)
        return;

    // Check this rigidbody's collider against all other colliders
    for (int i = 0; i < ColliderCount; ++i)
    {
        // Skip checking against our own collider
        if (GameColliders[i] == collider)
            continue;

        // Check for collision with this collider
        if (GameColliders[i]->Colliding(collider))
        {
            // Set grounded if colliding with something below us
            grounded = true;
            return;
        }
    }
}

RigidBody::RigidBody(Transform2D *transform, Collider *collider, float gravity) : transform(transform), collider(collider), gravity(gravity), velocity({0.0f, 0.0f}), grounded(false) {}

RigidBody::RigidBody(Transform2D * transform, Collider * collider) : transform(transform), collider(collider), gravity(DEFAULT_GRAVITY), velocity({0.0f, 0.0f}), grounded(false) {}

void RigidBody::ApplyPhysics()
{
    if (grounded) grounded = false;
    CheckCollisions();   // Check for collisions with other objects
    ApplyGravity();
    transform->position.x += velocity.x * FIXED_DELTA_TIME;
    transform->position.y += velocity.y * FIXED_DELTA_TIME;
}