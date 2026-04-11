#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include "box2d/box2d.h"
#include "entity.h"
#include "platform.h"
#include "raylib.h"

#define MAX_ENTITIES 128
#define MAX_PLATFORMS 128

extern Camera MainCamera;
extern b2WorldId MainWorld;
extern std::vector<std::unique_ptr<Entity>> Entities;
extern std::vector<std::unique_ptr<Platform>> Platforms;

// Initialize variable required for game
void InitGame();
// Called every game frame
void UpdateGame();
// Called every FIXED_DELTA_TIME seconds
void FixedUpdateGame();
// Draws the frame
void DrawGame();
// Draws debug information
void DrawDebug();
// Clear memory no longer needed for game
void FinishGame();

// Create new entity. Use RemoveEntity or ClearEntities for memory clearance
Entity *CreateEntity();

// Add entity for drawing
bool AddEntity(std::unique_ptr<Entity> entity);
// Remove entity for drawing
void RemoveEntity(Entity* entity);
// Remove all the entities
void ClearEntities();

// Create new platform. Use RemovePlatform or ClearPlatforms for memory clearance
Platform *CreatePlatform();
// Create new platform. Use RemovePlatform or ClearPlatforms for memory clearance
Platform *CreatePlatform(b2Vec2 size, b2Vec2 position);

// Add platform for drawing (if debugging)
bool AddPlatform(std::unique_ptr<Platform> platform);
//Remove platform for drawing
void RemovePlatform(Platform* platform);
// Removes all the platform
void ClearPlatforms();

#endif