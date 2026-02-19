#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"
#include "settings.h"

// Stores the position, the scale and the rotation of an object
class Transform2D
{
public:
    Vector2 position, scale;
    int rotation;
    Transform2D();
};

// Renders the sprite of an object
class SpriteRenderer
{
private:
    Transform2D *transform;

public:
    Texture2D *texture;
    Rectangle spriteRec;
    Color color;
    SpriteRenderer(Transform2D *transform);
    void Draw();
};

class Collider
{
public:
    bool isRound;
    float width, height;
    Collider(float width = 1.0f, float height = 1.0f);
    Collider(bool isRound, float diameter = 1.0f);
};

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
    RigidBody(Transform2D *transform, Collider *collider, float gravity = DEFAULT_GRAVITY);
    void ApplyPhysics();
};

#define PLAYER_GAMEPAD 0

class PlayerController
{
    RigidBody *rigidBody;
    bool enabled;

public:
    float moveSpeed;
    PlayerController(RigidBody *rigidBody = nullptr, float moveSpeed = 5.0f);
    void SetEnabled(bool value);
    void Update();
};

#endif