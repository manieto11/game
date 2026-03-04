#include "entity.h"
#include "settings.h"
#include "textures.h"

Entity::Entity() : body(), texture(&DefaultTexture)
{
    textureSource.x = 0.0f;
    textureSource.y = 0.0f;
    textureSource.width = 32.0f;
    textureSource.height = 32.0f;

    body.Set({1.0f, 1.0f}, 1.0f);
    body.friction = 0;
}

void DrawEntity(Entity *entity)
{
    Vector2 offset = PIXELS_PER_UNIT / 2.0f * entity->body.width;

    Rectangle destination = {entity->body.position.x * PIXELS_PER_UNIT, -entity->body.position.y * PIXELS_PER_UNIT, entity->body.width.x * PIXELS_PER_UNIT, entity->body.width.y * PIXELS_PER_UNIT};

    //TraceLog(LOG_INFO, "Painting entity at {%.1f, %.1f}", destination.x, destination.y);

    DrawTexturePro(*entity->texture, entity->textureSource, destination, offset, - RAD2DEG * entity->body.rotation, WHITE);
}

void DrawEntityBorder(Entity *entity)
{
    Vector2 offset = PIXELS_PER_UNIT / 2.0f * entity->body.width,
        center = PIXELS_PER_UNIT * entity->body.position;

    center.y = -center.y;

    DrawLine(center.x + offset.x, center.y + offset.y, center.x - offset.x, center.y + offset.y, LIME);
    DrawLine(center.x + offset.x, center.y - offset.y, center.x - offset.x, center.y - offset.y, LIME);
    DrawLine(center.x + offset.x, center.y + offset.y, center.x + offset.x, center.y - offset.y, LIME);
    DrawLine(center.x - offset.x, center.y + offset.y, center.x - offset.x, center.y - offset.y, LIME);
}
