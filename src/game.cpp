#include "game.h"
#include "player.h"
#include "settings.h"
#include "textures.h"

Camera2D MainCamera;
World MainWorld({0.0f, -GAME_GRAVITY}, 4);
Entity *Entities[MAX_ENTITIES] = {nullptr};
int EntityCount = 0;
Platform *Platforms[MAX_PLATFORMS] = {nullptr};
int PlatformCount = 0;

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

    PlayerEntity = CreateEntity();

    CreatePlatform()->body.position = {0.0f, -2.0f};
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
    for (int i = 0; i < EntityCount; i++) {
        DrawEntity(Entities[i]);
    }

    for (int i = 0; i < PlatformCount; i++) {
        DrawPlatformBorders(Platforms[i]);
    }
}

void FinishGame()
{
    ClearEntities();
    ClearPlatforms();
}

Entity *CreateEntity()
{
    Entity *entity = new Entity();

    if (AddEntity(entity))
        return entity;

    delete entity;

    return nullptr;
}

bool AddEntity(Entity *entity)
{
    if (EntityCount >= MAX_ENTITIES) 
    {
        TraceLog(LOG_WARNING, "Not enough space for more entities!");
        return false;
    }

    MainWorld.Add(&entity->body);

    Entities[EntityCount] = entity;
    EntityCount++;

    return true;
}

void RemoveEntity(Entity *entity)
{
    for (int i = 0; i <  EntityCount; ++i)
    {
        if (Entities[i] != entity) 
            continue;

        MainWorld.Remove(&entity->body);

        delete Entities[i];
        EntityCount--;
        Entities[i] = Entities[EntityCount];

        return;
    }
}

void ClearEntities()
{
    for (int i = 0; i <  EntityCount; ++i)
    {
        delete Entities[i];
    }

    EntityCount = 0;
}

Platform *CreatePlatform()
{
    Platform *platform = new Platform();

    platform->body.mass = FLT_MAX;

    if (AddPlatform(platform))
        return platform;

    delete platform;
    return nullptr;
}

bool AddPlatform(Platform *platform)
{
    if (PlatformCount >= MAX_PLATFORMS) 
    {
        TraceLog(LOG_WARNING, "Not enough space for more platforms!");
        return false;
    }

    MainWorld.Add(&platform->body);

    Platforms[PlatformCount] = platform;
    PlatformCount++;

    return true;
}

void RemovePlatform(Platform *platform)
{
    for (int i = 0; i <  PlatformCount; ++i)
    {
        if (Platforms[i] != platform) 
            continue;

        MainWorld.Remove(&platform->body);

        delete Platforms[i];
        PlatformCount--;
        Platforms[i] = Platforms[PlatformCount];

        return;
    }
}

void ClearPlatforms()
{
    for (int i = 0; i <  PlatformCount; ++i)
    {
        delete Platforms[i];
    }

    PlatformCount = 0;
}
