#ifndef COLLIDER_H
#define COLLIDER_H

#include "raylib.h"
#include "transform.h"

// Used for detecting collisions
class Collider
{
public:
    virtual bool Colliding(const Vector2 point) = 0;
};

// Collider with a rectangular shape
class BoxCollider : public Collider
{
private:
    Transform2D *transform;

public:
    BoxCollider();
    BoxCollider(BoxCollider &other);
    BoxCollider(Transform2D *transform);
    BoxCollider(Transform2D *transform, Rectangle rect);

    Rectangle rectangle;

    bool Colliding(const Vector2 point);
};

#endif