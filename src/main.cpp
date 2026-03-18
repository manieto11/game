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

    float fixedUpdate = 0.0f;

    Shader pixelShader = LoadShaderFromMemory(baseVertexShader, pixelizerFragmentShader);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

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
            UnloadRenderTexture(target);
            target = LoadRenderTexture(screenWidth, screenHeight);
            SetShaderValue(pixelShader, GetShaderLocation(pixelShader, "renderWidth"), &screenWidth, SHADER_UNIFORM_INT);
            SetShaderValue(pixelShader, GetShaderLocation(pixelShader, "renderHeight"), &screenHeight, SHADER_UNIFORM_INT);

        }

        if (steamEnabled)
            SteamAPI_RunCallbacks();

        fixedUpdate += GetFrameTime();
        //if (IsKeyPressed(KEY_ENTER))
        //    fixedUpdate += FIXED_DELTA_TIME;

        while (fixedUpdate >= FIXED_DELTA_TIME)
        {
            FixedUpdateGame();
            fixedUpdate -= FIXED_DELTA_TIME;
        }

        UpdateGame();

        BeginTextureMode(target);
        ClearBackground(SKYBLUE);

        BeginMode3D(MainCamera);
        DrawGame();
        EndMode3D();

        EndTextureMode(); 

        BeginDrawing();

        ClearBackground(SKYBLUE);

        BeginShaderMode(pixelShader);
        Rectangle dest = {0, 0, (float)target.texture.width, (float)-target.texture.height};
        DrawTextureRec(target.texture, dest, {0.0f, 0.0f}, WHITE);
        EndShaderMode();

#if DEBUG

        BeginMode3D(MainCamera);
        DrawDebug();
        EndMode3D();

        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, LIME);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIME);
        DrawFPS(10, 10);

#endif
        
        EndDrawing();
    }

    UnloadShader(pixelShader);

    UnloadRenderTexture(target);

    FinishGame();

    SaveSettings();

    SteamAPI_Shutdown();

    return 0;
}