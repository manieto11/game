#ifndef GAME_H
#define GAME_H

#include "box2d-lite/box2d_lite.h"
#include "entity.h"
#include "platform.h"
#include "custom_raylib.h"

#define MAX_ENTITIES 128
#define MAX_PLATFORMS 128

extern Camera2D MainCamera;
extern World MainWorld;
extern Entity *Entities[MAX_ENTITIES];
extern int EntityCount;
extern Platform *Platforms[MAX_PLATFORMS];
extern int PlatformCount;

// Initialize variable required for game
void InitGame();
// Called every game frame
void UpdateGame();
// Called every FIXED_DELTA_TIME seconds
void FixedUpdateGame();
// Draws the frame
void DrawGame();
// Clear memory no longer needed for game
void FinishGame();

// Create new entity. Use RemoveEntity or ClearEntities for memory clearance
Entity *CreateEntity();
// Add entity for drawing
bool AddEntity(Entity *entity);
//Remove entity for drawing
void RemoveEntity(Entity* entity);
// Removes all the entities
void ClearEntities();

// Create new platform. Use RemovePlatform or ClearPlatforms for memory clearance
Platform *CreatePlatform();
// Create new platform. Use RemovePlatform or ClearPlatforms for memory clearance
Platform *CreatePlatform(Vector2 size, Vector2 position);
// Add platform for drawing (if debugging)
bool AddPlatform(Platform *platform);
//Remove platform for drawing
void RemovePlatform(Platform* platform);
// Removes all the platform
void ClearPlatforms();

#endif