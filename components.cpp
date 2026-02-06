#include "components.h"
#include "settings.h"
#include "textures.h"

Transform2D::Transform2D()
{
    position = {0, 0};
    scale = {1, 1};
    rotation = 0;
}

SpriteRenderer::SpriteRenderer(Transform2D *transform)
{
    texture = &DefaultTexture;
    spriteRec = {0, 0, PIXELS_PER_UNIT, PIXELS_PER_UNIT};
    color = WHITE;
}

void SpriteRenderer::Draw()
{
    Rectangle dest;
    dest.x = ((int)transform->position.x) * PIXELS_PER_UNIT;
    dest.y = (-(int)transform->position.y) * PIXELS_PER_UNIT;
    dest.width = ((int)transform->scale.x) * PIXELS_PER_UNIT;
    dest.height = (-(int)transform->scale.y) * PIXELS_PER_UNIT;
    DrawTexturePro(*texture, spriteRec, dest, {(transform->scale.x) * PIXELS_PER_UNIT, (-transform->scale.y) * PIXELS_PER_UNIT / 2}, transform->rotation, color);
}
