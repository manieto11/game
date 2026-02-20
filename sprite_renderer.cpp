#include "sprite_renderer.h"
#include "settings.h"
#include "textures.h"

SpriteRenderer::SpriteRenderer(Transform2D *transform) : transform(transform)
{
    texture = &DefaultTexture;
    spriteRec = {0, 0, PIXELS_PER_UNIT, PIXELS_PER_UNIT};
    color = WHITE;
}

void SpriteRenderer::Draw()
{
    Rectangle dest;
    dest.x = (transform->position.x + transform->scale.y / 2) * PIXELS_PER_UNIT;
    dest.y = (-(transform->position.y + transform->scale.y)) * PIXELS_PER_UNIT;
    dest.width = (transform->scale.x) * PIXELS_PER_UNIT;
    dest.height = (transform->scale.y) * PIXELS_PER_UNIT;

    if (texture == nullptr or !IsTextureValid(*texture))
        TraceLog(LOG_WARNING, "Texture error!");
    DrawTexturePro(*texture, spriteRec, dest, {(transform->scale.x) * PIXELS_PER_UNIT, (-transform->scale.y) * PIXELS_PER_UNIT / 2}, transform->rotation, color);
}