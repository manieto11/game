#include "game.h"
#include "box2d/box2d.h"
#include "settings.h"
#include "window.h"

#include <iostream>

b2WorldId MainWorld;

void InitGame()
{
    if (!InitWindow(screenWidth, screenHeight, GAME_TITLE))
    {
        std::cerr << "Failed to create window" << std::endl;
    }

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -GAME_GRAVITY};

    MainWorld = b2CreateWorld(&worldDef);
}

void UpdateGame()
{
    
}

void FixedUpdateGame()
{
    b2World_Step(MainWorld, FIXED_DELTA_TIME, 4);
}

void DrawGame()
{
    
}

void FinishGame()
{
    b2DestroyWorld(MainWorld);
}

bool GameRunning()
{
    return !WindowShouldClose();
}
