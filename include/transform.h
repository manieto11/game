#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "raylib.h"

// Stores the position, the scale and the rotation of an object
class Transform2D
{
public:
    Vector2 position, scale;
    int rotation;
    Transform2D();
    Transform2D(int x, int y);
    Transform2D(float x, float y);

    void Move(int x, int y);
    void Move(float x, float y);

    void Resize(float sizeMultiplier);
    void Resize(float sizeMultiplierX, float sizeMultiplierY);

    void Rotate(int degrees);
};

#endif