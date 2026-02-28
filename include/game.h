#ifndef GAME_H
#define GAME_H

#include "box2d-lite/box2d_lite.h"
#include "entity.h"
#include "raylib.h"

#define MAX_ENTITIES 128

extern Camera2D MainCamera;
extern World MainWorld;
extern Entity *Entities[MAX_ENTITIES];

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

#endif