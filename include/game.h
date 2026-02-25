#ifndef GAME_H
#define GAME_H

#include "game_object.h"
#include "player.h"
#include "level.h"
#include "collider.h"
#include "raylib.h"

#define MAX_GAME_OBJECTS 64
#define MAX_COLLIDERS 128

extern Camera2D *MainCamera;
extern Player *MainPlayer;
extern GameObject *GameObjects[MAX_GAME_OBJECTS];
extern LevelManager *MainLevelManager;
extern Texture2D LevelTileset;
extern Collider *GameColliders[MAX_COLLIDERS];
extern int ColliderCount;

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

// Adds collider to physics
bool AddCollider(Collider* collider);
// Removes collider from physics
void RemoveCollider(Collider* collider);
// Removes collider from physics according to the index
void RemoveCollider(int index);
// Removes all the colliders from physics
void ClearColliders();

// Loads next level and changes the colliders
bool NextLevel();

#endif