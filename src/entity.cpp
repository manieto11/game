#include "entity.h"
#include "settings.h"
#include "textures.h"

Entity::Entity() : body(), texture(&DefaultTexture), size({1.0f, 1.0f})
{
    textureSource.x = 0.0f;
    textureSource.y = 0.0f;
    textureSource.width = 32.0f;
    textureSource.height = 32.0f;
}

void DrawEntity(Entity *entity)
{
    Vector2 position = b2Body_GetPosition(entity->body);
    Vector2 offset = PIXELS_PER_UNIT / 2.0f * entity->size;

    Rectangle destination = {position.x * PIXELS_PER_UNIT, -position.y * PIXELS_PER_UNIT, entity->size.x * PIXELS_PER_UNIT, entity->size.y * PIXELS_PER_UNIT};

    //TraceLog(LOG_INFO, "Painting entity at {%.1f, %.1f}", destination.x, destination.y);

    DrawTexturePro(*entity->texture, entity->textureSource, destination, offset, - RAD2DEG, WHITE);
}

void DrawEntityBorder(Entity *entity)
{
    Vector2 offset = PIXELS_PER_UNIT / 2.0f * entity->size,
        center = PIXELS_PER_UNIT * b2Body_GetPosition(entity->body);

    center.y = -center.y;

    DrawLine(center.x + offset.x, center.y + offset.y, center.x - offset.x, center.y + offset.y, LIME);
    DrawLine(center.x + offset.x, center.y - offset.y, center.x - offset.x, center.y - offset.y, LIME);
    DrawLine(center.x + offset.x, center.y + offset.y, center.x + offset.x, center.y - offset.y, LIME);
    DrawLine(center.x - offset.x, center.y + offset.y, center.x - offset.x, center.y - offset.y, LIME);
}
