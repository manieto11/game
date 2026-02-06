#include "game.h"
#include "raylib.h"
#include "settings.h"
#include "steam/steam_api.h"

int main()
{
    SetTraceLogLevel(LOG_WARNING);

    LoadSettings();

    InitWindow(screenWidth, screenHeight, GAME_TITLE);
    if (fullscreen)
        SetWindowState(FLAG_FULLSCREEN_MODE);
    SetExitKey(KEY_NULL);

    steamEnabled = SteamAPI_Init();
    if (!steamEnabled)
        TraceLog(LOG_WARNING, "Steam API not enabled!");

    InitGame();

    while (!WindowShouldClose())
    {
        if (steamEnabled) SteamAPI_RunCallbacks();

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode2D(*MainCamera);
        
        EndMode2D();
        EndDrawing();
    }

    FinishGame();

    SaveSettings();
}