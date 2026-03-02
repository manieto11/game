#ifndef ENTITY_H
#define ENTITY_H

#include "box2d-lite/box2d_lite.h"
#include "custom_raylib.h"

struct Entity
{
	Body body;
	Rectangle textureSource;
	Texture2D *texture;

	Entity();
	Entity(Vector2 position);
};

// Draw the entity
void DrawEntity(Entity *entity);
// Draw the entity's border for debugging
void DrawEntityBorder(Entity *entity);

#endif