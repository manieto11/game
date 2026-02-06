#ifndef COMPONENT_H
#define COMPONENT_H

#include "raylib.h"

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
    Texture2D* texture;
    Rectangle spriteRec;
    Color color;
    SpriteRenderer(Transform2D *transform);
    void Draw();
};

#endif