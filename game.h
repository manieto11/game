#ifndef GAME_H
#define GAME_H

#include "raylib.h"

Camera2D *MainCamera;

// Initialize variable required for game
void InitGame();
// Clear memory no longer needed for game
void FinishGame();

#endif