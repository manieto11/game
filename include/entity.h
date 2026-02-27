#ifndef ENTITY_H
#define ENTITY_H

#include "box2d-lite/box2d_lite.h"
#include "custom_raylib.h"

typedef struct Entity
{
	Body body;
	Vec2 size;
	Texture2D *texture;
} Entity;

void DrawEntity(Entity &entity);
void DrawEntityBorder(Entity &entity);

#endif