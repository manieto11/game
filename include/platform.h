#ifndef PLATFORM_H
#define PLATFORM_H

#include "box2d-lite/box2d_lite.h"

struct Platform
{
    Body body;

	Platform();
	Platform(World *world);
};

void DrawPlatformBorders(const Platform* platform);

#endif