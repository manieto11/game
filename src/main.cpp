#include "game.h"
#include "raylib.h"
#include "shaders.h"
#include "settings.h"
#include "steam/steam_api.h"

int main()
{
    InitGame();

    steamEnabled = SteamAPI_Init();
    if (!steamEnabled)
        TraceLog(LOG_WARNING, "Steam API not enabled!");

    float fixedUpdateAccumulator = 0.0f;
#if DEBUG == 0
    Shader pixelShader = LoadShaderFromMemory(baseVertexShader, pixelizerFragmentShader);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
#endif

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleBorderlessWindowed();
        }

        if (IsWindowResized())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

#if DEBUG == 0
            UnloadRenderTexture(target);
            target = LoadRenderTexture(screenWidth, screenHeight);
            SetShaderValue(pixelShader, GetShaderLocation(pixelShader, "renderWidth"), &screenWidth, SHADER_UNIFORM_INT);
            SetShaderValue(pixelShader, GetShaderLocation(pixelShader, "renderHeight"), &screenHeight, SHADER_UNIFORM_INT);
#endif
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

#if DEBUG
        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(MainCamera);
        DrawDebug();
        DrawGame();
        EndMode3D();

        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, LIME);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIME);
        DrawFPS(10, 10);
        
        EndDrawing();
#else
        BeginTextureMode(target);
        ClearBackground(SKYBLUE);

        BeginMode3D(MainCamera);
        DrawGame();
        EndMode3D();

        EndTextureMode(); 

        BeginDrawing();

        BeginShaderMode(pixelShader);
        Rectangle dest = {0, 0, (float)target.texture.width, (float)-target.texture.height};
        DrawTextureRec(target.texture, dest, {0.0f, 0.0f}, WHITE);
        EndShaderMode();
        
        EndDrawing();
#endif
    }

#if DEBUG == 0
    UnloadShader(pixelShader);

    UnloadRenderTexture(target);
#endif

    FinishGame();

    SaveSettings();

    SteamAPI_Shutdown();

    return 0;
}