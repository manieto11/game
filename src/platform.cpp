#include "platform.h"
#include "custom_raylib.h"
#include "settings.h"

Platform::Platform()
{
    body.Set({1.0f, 1.0f}, FLT_MAX);
}

Platform::Platform(World *world)
{
    body.Set({1.0f, 1.0f}, FLT_MAX);

    world->Add(&body);
}

void DrawPlatformBorders(const Platform* platform)
{
    Vec2 offset = PIXELS_PER_UNIT / 2.0f * platform->body.width,
        center = PIXELS_PER_UNIT * platform->body.position;

    center.y = -center.y;

    DrawLine(center.x + offset.x, center.y + offset.y, center.x - offset.x, center.y + offset.y, DARKBLUE);
    DrawLine(center.x + offset.x, center.y - offset.y, center.x - offset.x, center.y - offset.y, DARKBLUE);
    DrawLine(center.x + offset.x, center.y + offset.y, center.x + offset.x, center.y - offset.y, DARKBLUE);
    DrawLine(center.x - offset.x, center.y + offset.y, center.x - offset.x, center.y - offset.y, DARKBLUE);
}
