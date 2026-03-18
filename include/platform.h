#ifndef PLATFORM_H
#define PLATFORM_H

#include "box2d/box2d.h"

struct Platform
{
    b2BodyId body;
	const b2Vec2 size;

	Platform();
	Platform(b2Vec2 size);
};

void DrawPlatform(const Platform* platform);
void DrawPlatformBorders(const Platform* platform);

#endif