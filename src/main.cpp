#include "game.h"
#include "raylib.h"
#include "settings.h"
#include "steam/steam_api.h"

int main()
{
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

        UpdateGame();

        BeginDrawing();
        
        ClearBackground(SKYBLUE);

        BeginMode3D(MainCamera);
        DrawGame();
        EndMode3D();

#if DEBUG

        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, LIME);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIME);
        DrawFPS(10, 10);

#endif

        EndDrawing();
    }

    FinishGame();

    SaveSettings();

    SteamAPI_Shutdown();

    return 0;
}