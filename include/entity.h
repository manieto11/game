#ifndef ENTITY_H
#define ENTITY_H

#include "box2d/box2d.h"
#include "custom_raylib.h"

struct Entity
{
	b2BodyId body;
	Rectangle textureSource;
	Texture2D *texture;
	const Vector2 size;

	Entity();
	Entity(Vector2 size);
};

// Draw the entity
void DrawEntity(Entity *entity);
// Draw the entity's border for debugging
void DrawEntityBorder(Entity *entity);

#endif