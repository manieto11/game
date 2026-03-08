#ifndef SETTINGS_H
#define SETTINGS_H

#include "box2d/box2d.h"
#include "raylib.h"

// Title of the game's window
#define GAME_TITLE "game"
// Current version of the game
#define VERSION "early_dev"
// Time in seconds between physics' updates
#define FIXED_DELTA_TIME 0.02f
// Number of pixels considered a unit for drawing textures
#define PIXELS_PER_UNIT 32
// The objects' gravity force
#define GAME_GRAVITY 8.0f
// The default gamepad of the player
#define PLAYER_GAMEPAD 0
// The player's speed on the ground
#define PLAYER_SPEED 7.0f * speedMultiplier
// The player's jump force
#define PLAYER_JUMP_FORCE 7.0f * jumpMultiplier
// The player's acceleration in the air
#define PLAYER_AIR_ACCELERATION 10.0f * speedMultiplier * GetFrameTime()
// Time for the player in seconds to jump after falling of an edge
#define PLAYER_COYOTE_TIME 0.2f
// Time for the player to be able to jump again
#define PLAYER_DOUBLE_JUMP_DELAY 0.2f

// Set to 0 to hide debug information, set to 1 otherwise to show it
#define DEBUG 1

// The width of the game window
inline int screenWidth = 800;
// The height of the game window
inline int screenHeight = 600;

// The camera zoom
inline float scaleMultiplier = 1.0f;
// A multiplier for the player speed
inline float speedMultiplier = 1.0f;
// A multiplier for the player jump
inline float jumpMultiplier = 1.0f;

// True if Steam is working
inline bool steamEnabled = false;
// True if the game is currently in fullscreen
inline bool fullscreen = false;

// Key used to move left
inline KeyboardKey movingLeftKey = KEY_A, 
    movingLeftKeySecondary = KEY_LEFT;

// Key used to move right
inline KeyboardKey movingRightKey = KEY_D,
    movingRightKeySecondary = KEY_RIGHT;

//Load the user settings. Call SaveSettings before finishing execution to keep any changes
void LoadSettings();
//Save the user settings
void SaveSettings();

#endif