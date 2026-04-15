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

    float fixedUpdateAccumulator = 0.0f;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleBorderlessWindowed();
            fullscreen = !fullscreen;
        }

        if (IsWindowResized())
        {
            MainCamera.offset = {
                static_cast<float>(GetScreenWidth()) * 0.5f,
                static_cast<float>(GetScreenHeight()) * 0.5f,
            };
            MainCamera.zoom = static_cast<float>(GetScreenWidth()) / static_cast<float>(screenWidth);
        }

        if (steamEnabled)
            SteamAPI_RunCallbacks();

        fixedUpdateAccumulator += GetFrameTime();

        while (fixedUpdateAccumulator >= FIXED_DELTA_TIME)
        {
            FixedUpdateGame();
            fixedUpdateAccumulator -= FIXED_DELTA_TIME;
        }

        UpdateGame();

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode2D(MainCamera);
        DrawGame();
#if DEBUG
        DrawDebug();
#endif
        EndMode2D();

#if DEBUG
        DrawLine(0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, LIME);
        DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), LIME);
        DrawFPS(10, 10);
#endif
        
        EndDrawing();
    }

    FinishGame();

    SteamAPI_Shutdown();

    return 0;
}