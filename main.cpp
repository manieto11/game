#include "game.h"
#include "raylib.h"
#include "settings.h"
#include "steam/steam_api.h"

int main()
{
    SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, GAME_TITLE);
    if (fullscreen)
        SetWindowState(FLAG_FULLSCREEN_MODE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    InitGame();

    steamEnabled = SteamAPI_Init();
    if (!steamEnabled)
        TraceLog(LOG_WARNING, "Steam API not enabled!");

    float fixedUpdate = 0.0f;

    while (!WindowShouldClose())
    {
        if (steamEnabled)
            SteamAPI_RunCallbacks();

        fixedUpdate += GetFrameTime();

        while (fixedUpdate >= FIXED_DELTA_TIME)
        {
            FixedUpdateGame();
            fixedUpdate -= FIXED_DELTA_TIME;
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode2D(*MainCamera);
        UpdateGame();
        EndMode2D();
        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, LIME);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIME);
        DrawFPS(10, 10);
        EndDrawing();
    }

    FinishGame();

    SaveSettings();

    SteamAPI_Shutdown();

    return 0;
}