#include "game.h"
#include "steam/steam_api.h"
#include <iostream>

int main()
{
    InitGame();

    bool steamEnabled = SteamAPI_Init();
    if (!steamEnabled)
        //TraceLog(LOG_WARNING, "Steam API not enabled!");
        std::cerr << "Steam API not enabled!" << std::endl;

    float fixedUpdate = 0.0f;

    while (GameRunning())
    {
        if (steamEnabled)
            SteamAPI_RunCallbacks();

        // fixedUpdate += GetFrameTime();

        while (fixedUpdate >= FIXED_DELTA_TIME)
        {
            FixedUpdateGame();
            fixedUpdate -= FIXED_DELTA_TIME;
        }

        UpdateGame();

        DrawGame();
    }

    FinishGame();

    SteamAPI_Shutdown();

    return 0;
}