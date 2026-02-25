#include "sprite_renderer.h"
#include "settings.h"
#include "textures.h"

SpriteRenderer::SpriteRenderer(Transform2D *transform) : transform(transform), textureValid(false)
{
    texture = &DefaultTexture;
    spriteRec = {0, 0, PIXELS_PER_UNIT, PIXELS_PER_UNIT};
    color = WHITE;
    textureValid = (texture != nullptr && IsTextureValid(*texture));
}

void SpriteRenderer::SetTexture(Texture2D *newTexture)
{
    texture = newTexture;
    textureValid = (texture != nullptr && IsTextureValid(*texture));
    
    if (!textureValid)
        TraceLog(LOG_WARNING, "Invalid texture set in SpriteRenderer!");
}

void SpriteRenderer::Draw()
{
    // Early return if texture is invalid
    if (!textureValid)
        return;
    
    // Cache commonly used values
    const float scaleX = transform->scale.x * PIXELS_PER_UNIT;
    const float scaleY = transform->scale.y * PIXELS_PER_UNIT;
    
    // Calculate destination rectangle
    Rectangle dest = {
        (transform->position.x + transform->scale.y / 2) * PIXELS_PER_UNIT,
        -(transform->position.y + transform->scale.y) * PIXELS_PER_UNIT,
        scaleX,
        scaleY
    };
    
    // Calculate origin for rotation
    Vector2 origin = {scaleX, -scaleY / 2};
    
    DrawTexturePro(*texture, spriteRec, dest, origin, transform->rotation, color);
}