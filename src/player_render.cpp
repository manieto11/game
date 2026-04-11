#include "player_internal.h"

void DrawPlayer()
{
    const Rectangle bodySource = {lookingRight ? 0.0f : 32.0f, 0.0f, 32.0f, 32.0f};
    const Rectangle legSource = {64.0f, 0.0f, 32.0f, 32.0f};

    const Rectangle bodyDest = {bodyPos.x * PIXELS_PER_UNIT, -bodyPos.y * PIXELS_PER_UNIT, PlayerEntity->size.x * PIXELS_PER_UNIT, PlayerEntity->size.y * PIXELS_PER_UNIT};
    const Rectangle leftLegDest = {leftLegPos.x * PIXELS_PER_UNIT, -leftLegPos.y * PIXELS_PER_UNIT, PlayerEntity->size.x * PIXELS_PER_UNIT, PlayerEntity->size.y * PIXELS_PER_UNIT};
    const Rectangle rightLegDest = {rightLegPos.x * PIXELS_PER_UNIT, -rightLegPos.y * PIXELS_PER_UNIT, PlayerEntity->size.x * PIXELS_PER_UNIT, PlayerEntity->size.y * PIXELS_PER_UNIT};

    DrawTexturePro(PlayerTexture, bodySource, bodyDest, {bodyDest.width / 2.0f, bodyDest.height / 2.0f}, bodyRotation, WHITE);
    DrawTexturePro(PlayerTexture, legSource, leftLegDest, {leftLegDest.width / 2.0f, leftLegDest.height}, leftLegRotation, WHITE);
    DrawTexturePro(PlayerTexture, legSource, rightLegDest, {rightLegDest.width / 2.0f, rightLegDest.height}, rightLegRotation, WHITE);
}

void DrawPlayerDebug()
{
    const Rectangle groundedRect = GroundProbeRect(playerPosition);
    DrawRectangleLines(groundedRect.x * PIXELS_PER_UNIT, -groundedRect.y * PIXELS_PER_UNIT, groundedRect.width * PIXELS_PER_UNIT, groundedRect.height * PIXELS_PER_UNIT, grounded ? LIME : RED);

    const Rectangle leftRect = WallProbeRect(playerPosition, false);
    const Rectangle rightRect = WallProbeRect(playerPosition, true);
    DrawRectangleLines(leftRect.x * PIXELS_PER_UNIT, (-leftRect.y - 0.95f * PlayerEntity->size.y) * PIXELS_PER_UNIT, leftRect.width * PIXELS_PER_UNIT, leftRect.height * PIXELS_PER_UNIT, bodyUp == kWallUpLeft ? LIME : RED);
    DrawRectangleLines(rightRect.x * PIXELS_PER_UNIT, (-rightRect.y - 0.95f * PlayerEntity->size.y) * PIXELS_PER_UNIT, rightRect.width * PIXELS_PER_UNIT, rightRect.height * PIXELS_PER_UNIT, bodyUp == kWallUpRight ? LIME : RED);
}