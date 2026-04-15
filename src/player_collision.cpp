#include "player_internal.h"

#include <cmath>

namespace
{
    constexpr float kProbeThickness = 0.1f;

    Rectangle SurfaceProbeRect(const b2Vec2& position, const b2Vec2& up)
    {
        if (fabs(up.x) > 0.5f)
        {
            const float x = position.x - up.x * (0.5f * PlayerEntity->size.x + 0.05f);
            return {x, position.y - PlayerEntity->size.y * 0.95f / 2.0f, kProbeThickness, PlayerEntity->size.y * 0.95f};
        }

        const float y = position.y - up.y * (PlayerEntity->size.y / 2.0f + 0.05f);
        return {position.x - 0.5f * PlayerEntity->size.x, y, PlayerEntity->size.x, kProbeThickness};
    }
}

Rectangle GroundProbeRect(const b2Vec2& position)
{
    return SurfaceProbeRect(position, kFloorUp);
}

Rectangle WallProbeRect(const b2Vec2& position, bool rightSide)
{
    return SurfaceProbeRect(position, rightSide ? kWallUpRight : kWallUpLeft);
}