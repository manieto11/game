#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"

// Constant parameters

#define GAME_TITLE "game"

#define VERSION "early_dev"

#define FIXED_DELTA_TIME 0.02f

#define PIXELS_PER_UNIT 32

#define GAME_GRAVITY 8.0f

#define PLAYER_GAMEPAD 0

#define PLAYER_SPEED 2.0f * speed_multiplier

#define DEFAULT_GRID_SIZE 128

#define TILE_SIZE 32

#define DEFAULT_GRID_SCALE 1.0f 

// Configurable parameters

inline int screenWidth = 800, 
    screenHeight = 600;

inline float scaleMultiplier = 1.0f,
    speed_multiplier = 1.0f;

inline bool steamEnabled = false, 
    fullscreen = false;

inline KeyboardKey moving_left_key = KEY_A, 
    moving_left_key_secondary = KEY_LEFT, 
    moving_right_key = KEY_D,
    moving_right_key_secondary = KEY_RIGHT;

// Setting-related methods

//Load the user settings. Call LoadSettings before finishing execution to keep any changes.
void LoadSettings();
//Save the user settings.
void SaveSettings();

#endif