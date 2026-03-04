#ifndef PLATFORM_H
#define PLATFORM_H

#include "box2d/box2d.h"
#include "custom_raylib.h"

struct Platform
{
    b2BodyId body;
	const Vector2 size;

	Platform();
	Platform(Vector2 size);
};

void DrawPlatformBorders(const Platform* platform);

#endif