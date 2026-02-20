#ifndef GAME_H
#define GAME_H

#include "game_object.h"
#include "player.h"
#include "raylib.h"

#define MAX_GAME_OBJECTS 64

extern Camera2D *MainCamera;
extern Player *MainPlayer;
extern GameObject *gameObjects[MAX_GAME_OBJECTS];

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