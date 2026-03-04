#include "game_utils.h"

bool PointInsideBox(Vector2 topRight, Vector2 bottomLeft, Vector2 point)
{
    return (point.x < topRight.x && point.x > bottomLeft.x) && (point.y < topRight.y && point.y > bottomLeft.y);
}

void SetRange(float &value, float min, float max)
{
    if (value < min) 
        value = min;
    else if (value > max)
        value = max;
}
