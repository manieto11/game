#ifndef UTILS_H
#define UTILS_H

#include "custom_raylib.h"

// Return true if the point is inside the box defined by topRight and bottomLeft
bool PointInsideBox(Vector2 topRight, Vector2 bottomLeft, Vector2 point);

// Set value between \[min, max]
void SetRange(float& value, float min, float max);

#endif