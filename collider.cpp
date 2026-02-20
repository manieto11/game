#include "collider.h"

BoxCollider::BoxCollider() : transform(nullptr), rectangle({0.0f, 0.0f, 0.0f, 0.0f}) {}

BoxCollider::BoxCollider(BoxCollider &other)  : transform(other.transform), rectangle(other.rectangle) {}

BoxCollider::BoxCollider(Transform2D *transform) : transform(nullptr), rectangle({0.0f, 0.0f, 1.0f, 1.0f}) {}

BoxCollider::BoxCollider(Transform2D *transform, Rectangle rect) : transform(nullptr), rectangle(rect) {}

bool BoxCollider::Colliding(const Vector2 point)
{
    return false;
}

GridCollider::GridCollider() : grid(nullptr) {}

GridCollider::GridCollider(GridCollider &other) : grid(other.grid) {}

GridCollider::GridCollider(Grid *grid) : grid(grid) {}

GridCollider::GridCollider(Grid *grid, Vector2 origin) : grid(grid) {}

bool GridCollider::Colliding(const Vector2 point)
{
    return false;
}