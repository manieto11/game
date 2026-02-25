#ifndef COLLIDER_H
#define COLLIDER_H

#include "raylib.h"
#include "dual_grid_system.h"
#include "transform.h"

// Used for detecting collisions
class Collider
{
public:
    virtual bool Colliding(const Vector2 point) = 0;
    virtual bool Colliding(Collider *other) = 0;
    virtual void DrawDebug() = 0;
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
    bool Colliding(Collider *other);

    void DrawDebug();
};

class GridCollider : public Collider
{
private:
    Grid *grid;

public:
    GridCollider();
    GridCollider(GridCollider &other);
    GridCollider(Grid* grid);
    GridCollider(Grid* grid, Vector2 origin);

    bool Colliding(Collider *other);
    bool Colliding(const Vector2 point);
};

#endif