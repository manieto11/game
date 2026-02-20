#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "collider.h"
#include "transform.h"
#include "raylib.h"

class RigidBody
{
private:
    Transform2D *transform;
    Collider *collider;
    bool grounded;
    void ApplyGravity();

public:
    Vector2 velocity;
    float gravity;
    RigidBody(Transform2D *transform, Collider *collider);
    RigidBody(Transform2D *transform, Collider *collider, float gravity);
    void ApplyPhysics();
};

#endif