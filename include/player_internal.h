#ifndef PLAYER_INTERNAL_H
#define PLAYER_INTERNAL_H

#include <memory>
#include <vector>
#include "player.h"
#include "raymath.h"
#include "settings.h"
#include "textures.h"

extern Entity* PlayerEntity;

extern float inputX;
extern float elapsedCoyoteTime;
extern float lastJumpTime;
extern float step;
extern float bodyRotation;
extern float leftLegRotation;
extern float rightLegRotation;

extern bool doubleJump;
extern bool unlockedDoubleJump;
extern bool grounded;
extern bool wallClimbing;
extern bool unlockedWallClimbing;

extern b2Vec2 playerPosition;
extern b2Vec2 playerVelocity;
extern b2Vec2 bodyPos;
extern b2Vec2 leftLegPos;
extern b2Vec2 rightLegPos;
extern b2Vec2 bodyUp;

inline constexpr b2Vec2 kFloorUp = {0.0f, 1.0f};
inline constexpr b2Vec2 kWallUpLeft = {1.0f, 0.0f};
inline constexpr b2Vec2 kWallUpRight = {-1.0f, 0.0f};

Rectangle PlatformRect(const Platform* platform);
Rectangle GroundProbeRect(const b2Vec2& position);
Rectangle WallProbeRect(const b2Vec2& position, bool rightSide);

#endif