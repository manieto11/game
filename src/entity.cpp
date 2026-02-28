#include "entity.h"
#include "settings.h"
#include "textures.h"

void DrawEntity(Entity *entity)
{
    Vector2 offset = 16.0f * entity->body.width;

    Rectangle destination = {entity->body.position.x * PIXELS_PER_UNIT, -entity->body.position.y * PIXELS_PER_UNIT, entity->body.width.x * PIXELS_PER_UNIT, entity->body.width.y * PIXELS_PER_UNIT};

    //TraceLog(LOG_INFO, "Painting entity at {%.1f, %.1f}", destination.x, destination.y);

    DrawTexturePro(*entity->texture, entity->textureSource, destination, offset, DEG2RAD * entity->body.rotation, WHITE);
}

Entity::Entity() : body(), texture(&DefaultTexture)
{
    textureSource.x = 0.0f;
    textureSource.y = 0.0f;
    textureSource.width = 32.0f;
    textureSource.height = 32.0f;

    body.Set({1.0f, 1.0f}, 1.0f);
}
