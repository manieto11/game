#ifndef SETTINGS_H
#define SETTINGS_H

// Constant parameters

#define GAME_TITLE "game"

#define FIXED_DELTA_TIME 0.2f;

#define VERSION "early_dev"

#define PIXELS_PER_UNIT 32

// Configurable parameters

inline int screenWidth = 800, screenHeight = 600;

inline float scaleMultiplier = 1.0f;

inline bool steamEnabled = false, fullscreen = false;

// Setting-related methods

//Load the user settings. Call LoadSettings before finishing execution to keep any changes.
void LoadSettings();
//Save the user settings.
void SaveSettings();

#endif