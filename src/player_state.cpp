#include "player_internal.h"

Entity* PlayerEntity = nullptr;

float inputX = 0.0f;
float elapsedCoyoteTime = 0.0f;
float lastJumpTime = 0.0f;
float step = 0.0f;
float bodyRotation = 0.0f;
float leftLegRotation = 0.0f;
float rightLegRotation = 0.0f;

bool doubleJump = false;
bool unlockedDoubleJump = false;
bool grounded = false;
bool wallClimbing = false;
bool unlockedWallClimbing = true;

b2Vec2 playerPosition = {0.0f, 0.0f};
b2Vec2 playerVelocity = {0.0f, 0.0f};
b2Vec2 bodyPos = {0.0f, 0.0f};
b2Vec2 leftLegPos = {0.0f, 0.0f};
b2Vec2 rightLegPos = {0.0f, 0.0f};
b2Vec2 bodyUp = {0.0f, 1.0f};