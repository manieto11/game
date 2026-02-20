#include "transform.h"

Transform2D::Transform2D() : position({0.0f, 0.0f}), scale({1.0f, 1.0f}), rotation(0) {}

Transform2D::Transform2D(int x, int y) : position({(float)x, (float)y}), scale({1.0f, 1.0f}), rotation(0) {}

Transform2D::Transform2D(float x, float y) : position({x, y}), scale({1.0f, 1.0f}), rotation(0) {}

void Transform2D::Move(int x, int y)
{
    position.x += (float)x;
    position.y += (float)y;
}

void Transform2D::Move(float x, float y)
{
    position.x += x;
    position.y += y;
}

void Transform2D::Resize(float sizeMultiplier)
{
    scale.x *= sizeMultiplier;
    scale.y *= sizeMultiplier;
}

void Transform2D::Resize(float sizeMultiplierX, float sizeMultiplierY)
{
    scale.x *= sizeMultiplierX;
    scale.y *= sizeMultiplierY;
}

void Transform2D::Rotate(int degrees)
{
    rotation += degrees;
}
