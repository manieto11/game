#include "platform.h"
#include "raylib.h"
#include "settings.h"
#include <cmath>

static Vector2 RotateOffset(Vector2 v, float c, float s)
{
    return {v.x * c - v.y * s, v.x * s + v.y * c};
}

void DrawPlatform(const Platform *platform)
{
    b2Vec2 offset = PIXELS_PER_UNIT / 2.0f * platform->size,
        center = PIXELS_PER_UNIT * b2Body_GetPosition(platform->body);

    center.y = -center.y;

    Rectangle rec = {center.x, center.y, platform->size.x * PIXELS_PER_UNIT, platform->size.y * PIXELS_PER_UNIT};

    float rot = b2Rot_GetAngle(b2Body_GetRotation(platform->body));

    DrawRectanglePro(rec, {offset.x, offset.y}, - RAD2DEG * rot, DARKGRAY);
}

void DrawPlatformBorders(const Platform *platform)
{
    b2Vec2 center = PIXELS_PER_UNIT * b2Body_GetPosition(platform->body);
    center.y = -center.y;

    const float halfW = platform->size.x * PIXELS_PER_UNIT * 0.5f;
    const float halfH = platform->size.y * PIXELS_PER_UNIT * 0.5f;

    // Match DrawPlatform: Y is flipped in screen space, so rotation must be negated.
    //const float angle = -b2Rot_GetAngle(b2Body_GetRotation(platform->body));
    //const float c = cosf(angle);
    //const float s = sinf(angle);

    const float c = -b2Body_GetRotation(platform->body).c;
    const float s = b2Body_GetRotation(platform->body).s;

    Vector2 localTopLeft = {-halfW, -halfH};
    Vector2 localTopRight = {halfW, -halfH};
    Vector2 localBottomRight = {halfW, halfH};
    Vector2 localBottomLeft = {-halfW, halfH};

    Vector2 p0 = RotateOffset(localTopLeft, c, s);
    Vector2 p1 = RotateOffset(localTopRight, c, s);
    Vector2 p2 = RotateOffset(localBottomRight, c, s);
    Vector2 p3 = RotateOffset(localBottomLeft, c, s);

    p0.x += center.x; p0.y += center.y;
    p1.x += center.x; p1.y += center.y;
    p2.x += center.x; p2.y += center.y;
    p3.x += center.x; p3.y += center.y;

    DrawLineV(p0, p1, DARKBLUE);
    DrawLineV(p1, p2, DARKBLUE);
    DrawLineV(p2, p3, DARKBLUE);
    DrawLineV(p3, p0, DARKBLUE);
}

Platform::Platform() : size({1.0f, 1.0f})
{

}

Platform::Platform(b2Vec2 size) : size(size)
{
    
}
