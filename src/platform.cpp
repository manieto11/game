#include "platform.h"
#include "raylib.h"
#include "settings.h"

void DrawPlatformBorders(const Platform* platform)
{
    b2Vec2 offset = PIXELS_PER_UNIT / 2.0f * platform->size,
        center = PIXELS_PER_UNIT * b2Body_GetPosition(platform->body);

    center.y = -center.y;

    DrawRectangleLines(center.x - offset.x, center.y - offset.y, platform->size.x * PIXELS_PER_UNIT, platform->size.y * PIXELS_PER_UNIT, DARKBLUE);
}

Platform::Platform() : size({1.0f, 1.0f})
{

}

Platform::Platform(b2Vec2 size) : size(size)
{
    
}
