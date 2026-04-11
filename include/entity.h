#ifndef ENTITY_H
#define ENTITY_H

#include "box2d/box2d.h"
#include "raylib.h"

struct Entity
{
	b2BodyId body;
	Rectangle textureSource;
	Texture2D* texture;
	const b2Vec2 size;

	Entity();
	Entity(b2Vec2 size);
};

// Draw the entity
void DrawEntity(const Entity* entity);
// Draw the entity's border for debugging
void DrawEntityBorder(const Entity* entity);

#endif