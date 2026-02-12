#ifndef GAME_H
#define GAME_H

#define MAX_GAME_OBJECTS 64

#include "gameObject.h"
#include "player.h"
#include "raylib.h"
#include "settings.h"
#include "textures.h"

extern Camera2D *MainCamera;
extern Player *MainPlayer;
extern GameObject *gameObjects[MAX_GAME_OBJECTS];

// Initialize variable required for game
void InitGame();
// Called every game frame
void UpdateGame();
// Called every FIXED_DELTA_TIME seconds
void FixedUpdateGame();
// Clear memory no longer needed for game
void FinishGame();

#endif