#ifndef SPRITE_RENDERER
#define SPRITE_RENDERER

#include "raylib.h"
#include "transform.h"

// Renders the sprite of an object
class SpriteRenderer
{
private:
    Transform2D *transform;
    bool textureValid;

public:
    Texture2D *texture;
    Rectangle spriteRec;
    Color color;
    SpriteRenderer(Transform2D *transform);
    void SetTexture(Texture2D *newTexture);
    void Draw();
};

#endif