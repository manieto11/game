#ifndef SETTINGS_H
#define SETTINGS_H

#include "box2d/box2d.h"
// #include "raylib.h"

// Number of pixels considered a unit for drawing textures
#define PIXELS_PER_UNIT 32
// The objects' gravity force
#define GAME_GRAVITY 8.0f
// The input acceleration
#define INPUT_ACCELERATION 8.0f * GetFrameTime()
// The default gamepad of the player
#define PLAYER_GAMEPAD 0
// The player's speed on the ground
#define PLAYER_SPEED 5.0f * speedMultiplier
// The player's jump force
#define PLAYER_JUMP_HEIGHT 3.5f + extraJumpHeight
// Time for the player in seconds to jump after falling of an edge
#define PLAYER_COYOTE_TIME 0.2f
// Time for the player to be able to jump again
#define PLAYER_JUMP_DELAY 0.2f

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
inline float extraJumpHeight = 0.0f;

// True if Steam is working
inline bool steamEnabled = false;
// True if the game is currently in fullscreen
inline bool fullscreen = false;

// Key used to move left
// inline KeyboardKey movingLeftKey = KEY_A, 
//     movingLeftKeySecondary = KEY_LEFT;

// Key used to move right
// inline KeyboardKey movingRightKey = KEY_D,
//     movingRightKeySecondary = KEY_RIGHT;

//Load the user settings. Call SaveSettings before finishing execution to keep any changes
void LoadSettings();
//Save the user settings
void SaveSettings();

#endif