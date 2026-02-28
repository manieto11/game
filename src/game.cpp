#include "game.h"
#include "player.h"
#include "settings.h"
#include "textures.h"

Camera2D MainCamera;
World MainWorld({0.0f, -GAME_GRAVITY}, 4);
Entity *Entities[MAX_ENTITIES];

void InitGame()
{
    // SetTraceLogLevel(LOG_WARNING);

    InitWindow(screenWidth, screenHeight, GAME_TITLE " " VERSION);
    if (fullscreen)
        SetWindowState(FLAG_FULLSCREEN_MODE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    LoadSettings();
    LoadGameTextures();

    MainCamera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    MainCamera.rotation = 0;
    MainCamera.target = {0, 0};
    MainCamera.zoom = scaleMultiplier;

    Entities[0] = new Entity();
    Entities[0]->body.Set({1.0f, 1.0f}, 1.0f);

    Entities[1] = new Entity();
    Entities[1]->body.Set({32.0f, 1.0f}, FLT_MAX);
    Entities[1]->body.position = {0.0f, -4.0f};

    MainWorld.Add(&Entities[0]->body);
    MainWorld.Add(&Entities[1]->body);

    PlayerEntity = Entities[0];
}

void UpdateGame()
{
    UpdatePlayer();
}

void FixedUpdateGame()
{
    MainWorld.Step(FIXED_DELTA_TIME);
}

void DrawGame()
{
    DrawEntity(Entities[0]);
    DrawEntity(Entities[1]);
}

void FinishGame()
{
    delete Entities[0];
    delete Entities[1];
}
