#include "entity.h"
#include "settings.h"
#include "textures.h"

static Vector2 RotateOffset(Vector2 v, float c, float s)
{
    return {v.x * c - v.y * s, v.x * s + v.y * c};
}

Entity::Entity() : Entity({1.0f, 1.0f})
{
}

Entity::Entity(b2Vec2 size) : body(), texture(&DefaultTexture), size(size)
{
    textureSource.x = 0.0f;
    textureSource.y = 0.0f;
    textureSource.width = 32.0f;
    textureSource.height = 32.0f;
}

void DrawEntity(const Entity* entity)
{
    const b2Vec2 position = b2Body_GetPosition(entity->body);
    const b2Vec2 offset = PIXELS_PER_UNIT / 2.0f * entity->size;

    Rectangle destination = {position.x * PIXELS_PER_UNIT, -position.y * PIXELS_PER_UNIT, entity->size.x * PIXELS_PER_UNIT, entity->size.y * PIXELS_PER_UNIT};

    DrawTexturePro(*entity->texture, entity->textureSource, destination, {offset.x, offset.y}, 0.0f, WHITE);
}

void DrawEntityBorder(const Entity* entity)
{
    b2Vec2 center = PIXELS_PER_UNIT * b2Body_GetPosition(entity->body);
    center.y = -center.y;

    const float halfW = entity->size.x * PIXELS_PER_UNIT * 0.5f;
    const float halfH = entity->size.y * PIXELS_PER_UNIT * 0.5f;

    const float c = -b2Body_GetRotation(entity->body).c;
    const float s = b2Body_GetRotation(entity->body).s;

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

    DrawLineV(p0, p1, YELLOW);
    DrawLineV(p1, p2, YELLOW);
    DrawLineV(p2, p3, YELLOW);
    DrawLineV(p3, p0, YELLOW);
}
