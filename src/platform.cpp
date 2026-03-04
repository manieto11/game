#include "platform.h"
#include "custom_raylib.h"
#include "settings.h"

void DrawPlatformBorders(const Platform* platform)
{
    Vector2 offset = PIXELS_PER_UNIT / 2.0f * platform->size,
        center = PIXELS_PER_UNIT * b2Body_GetPosition(platform->body);

    center.y = -center.y;

    DrawLine(center.x + offset.x, center.y + offset.y, center.x - offset.x, center.y + offset.y, DARKBLUE);
    DrawLine(center.x + offset.x, center.y - offset.y, center.x - offset.x, center.y - offset.y, DARKBLUE);
    DrawLine(center.x + offset.x, center.y + offset.y, center.x + offset.x, center.y - offset.y, DARKBLUE);
    DrawLine(center.x - offset.x, center.y + offset.y, center.x - offset.x, center.y - offset.y, DARKBLUE);
}
