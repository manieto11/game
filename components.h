#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"

// Store the position, the scale and the rotation of an object
class Transform2D
{
public:
    Vector2 position, scale;
    int rotation;
};

#endif