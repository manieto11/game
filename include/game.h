#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include "box2d/box2d.h"
#include "entity.h"
#include "grid.h"
#include "raylib.h"

#define MAX_ENTITIES 128

extern Camera2D MainCamera;
extern b2WorldId MainWorld;
extern std::vector<std::unique_ptr<Entity>> Entities;
extern std::vector<std::unique_ptr<Grid>> Grids;

// Define camera dead zone width (player can move this far left/right before camera moves)
// Height is auto-calculated based on screen aspect ratio
#define CAMERA_DEADZONE_WIDTH 3.0f

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

// Update camera to follow player with deadzone
void UpdateCameraFollowPlayer();

// Create new entity. Use RemoveEntity or ClearEntities for memory clearance
Entity *CreateEntity();
// Create new entity. Use RemoveEntity or ClearEntities for memory clearance
Entity *CreateEntity(float x, float y);

// Add entity for drawing
bool AddEntity(std::unique_ptr<Entity> entity);
// Remove entity for drawing
void RemoveEntity(Entity* entity);
// Remove all the entities
void ClearEntities();

// Create new grid
Grid* CreateGrid(b2Vec2 cellSize, int gridWidth, int gridHeight, bool hasCollision);
// Add grid for drawing and physics
bool AddGrid(std::unique_ptr<Grid> grid);
// Remove grid
void RemoveGrid(Grid* grid);
// Clear all grids
void ClearGrids();

#endif