#include "game.h"
#include "materials.h"
#include "meshes.h"
#include "player.h"
#include "settings.h"
#include "textures.h"

Camera MainCamera;
b2WorldId MainWorld;
std::vector<Entity*> Entities;
std::vector<Platform*> Platforms;

void InitGame()
{
#if DEBUG == 0
    SetTraceLogLevel(LOG_WARNING);
#endif

    LoadSettings();

    InitWindow(screenWidth, screenHeight, GAME_TITLE " " VERSION);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    if (fullscreen)
        SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
#if DEBUG == 0
    SetExitKey(KEY_NULL);
#endif
    SetTargetFPS(60);

    LoadGameTextures();
    LoadGameMaterials();
    LoadGameMeshes();

    MainCamera.position = {0.0f, 0.0f, -10.0f};
    MainCamera.target = {0.0f, 0.0f, 0.0f};
    MainCamera.up = {0.0f, -1.0f, 0.0f};
    MainCamera.fovy = 16.0f * PIXELS_PER_UNIT;
    MainCamera.projection = CAMERA_ORTHOGRAPHIC;

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -GAME_GRAVITY};

    MainWorld = b2CreateWorld(&worldDef);

    PlayerEntity = CreateEntity();

    CreatePlatform({1.0f, 10.0f}, {0.0f, -0.5f});
    CreatePlatform({14.0f, 1.0f}, {0.0f, -5.0f});
}

void UpdateGame()
{
    UpdatePlayer(Platforms);
}

void FixedUpdateGame()
{
    b2World_Step(MainWorld, FIXED_DELTA_TIME, 4);
}

void DrawGame()
{
    for (size_t i = 0; i < Entities.size(); i++)
    {
        if (Entities[i] == PlayerEntity) continue;

        DrawEntity(Entities[i]);
    }

    for (size_t i = 0; i < Platforms.size(); i++)
    {
        DrawPlatform(Platforms[i]);
    }

    DrawPlayer();
}

void DrawDebug()
{
    for (size_t i = 0; i < Entities.size(); i++)
    {
        DrawEntityBorder(Entities[i]);
    }

    for (size_t i = 0; i < Platforms.size(); i++)
    {
        DrawPlatformBorders(Platforms[i]);
    }

    DrawPlayerDebug();
}

void FinishGame()
{
    ClearEntities();
    ClearPlatforms();

    b2DestroyWorld(MainWorld);

    UnloadGameMaterials();
    UnloadGameTextures();
}

Entity *CreateEntity()
{
    Entity *entity = new Entity();

    if (AddEntity(entity))
    {

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.gravityScale = 1.0f;

        b2MotionLocks motionLocks;
        motionLocks.linearX = false;
        motionLocks.linearY = false;
        motionLocks.angularZ = true;

        bodyDef.motionLocks = motionLocks;

        bodyDef.isEnabled = true;
        bodyDef.isAwake = true;

        entity->body = b2CreateBody(MainWorld, &bodyDef);

        b2Polygon entityBox = b2MakeBox(entity->size.x / 2.0f, entity->size.y / 2.0f);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.material.friction = 0.0f;
        b2CreatePolygonShape(entity->body, &shapeDef, &entityBox);

        return entity;
    }

    delete entity;

    return nullptr;
}

bool AddEntity(Entity *entity)
{
    if (Entities.size() >= MAX_ENTITIES)
    {
        TraceLog(LOG_WARNING, "Not enough space for more entities!");
        return false;
    }

    Entities.push_back(entity);

    return true;
}

void RemoveEntity(Entity *entity)
{
    if (entity == nullptr)
        return;

    for (size_t i = 0; i < Entities.size(); ++i)
    {
        if (Entities[i] != entity)
            continue;

        b2DestroyBody(entity->body);

        delete Entities[i];
        Entities[i] = Entities.back();
        Entities.pop_back();

        break;
    }
}

void ClearEntities()
{
    for (int i = 0; i < Entities.size(); ++i)
    {
        b2DestroyBody(Entities[i]->body);
        delete Entities[i];
    }

    Entities.clear();
}

Platform *CreatePlatform()
{
    Platform *platform = new Platform();

    if (AddPlatform(platform))
    {

        b2BodyDef bodyDef = b2DefaultBodyDef();

        platform->body = b2CreateBody(MainWorld, &bodyDef);

        b2Polygon groundBox = b2MakeBox(0.5f, 0.5f);
        b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        groundShapeDef.material.friction = 0.0f;
        b2CreatePolygonShape(platform->body, &groundShapeDef, &groundBox);

        return platform;
    }

    delete platform;

    return nullptr;
}

Platform *CreatePlatform(b2Vec2 size, b2Vec2 position)
{
    Platform *platform = new Platform(size);

    if (AddPlatform(platform))
    {

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = {position.x, position.y};

        platform->body = b2CreateBody(MainWorld, &bodyDef);

        b2Polygon groundBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
        b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(platform->body, &groundShapeDef, &groundBox);

        return platform;
    }

    delete platform;

    return nullptr;
}

/*Platform *CreatePlatform(b2Vec2 size, b2Vec2 position, float angle)
{
    Platform *platform = new Platform(size);

    if (AddPlatform(platform))
    {

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = {position.x, position.y};
        bodyDef.rotation = b2MakeRot(DEG2RAD * angle);

        platform->body = b2CreateBody(MainWorld, &bodyDef);

        b2Polygon groundBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
        b2ShapeDef groundShapeDef = b2DefaultShapeDef();
        b2CreatePolygonShape(platform->body, &groundShapeDef, &groundBox);

        return platform;
    }

    delete platform;

    return nullptr;
}*/

bool AddPlatform(Platform *platform)
{
    if (Platforms.size() >= MAX_PLATFORMS)
    {
        TraceLog(LOG_WARNING, "Not enough space for more platforms!");
        return false;
    }

    Platforms.push_back(platform);

    return true;
}

void RemovePlatform(Platform *platform)
{
    for (size_t i = 0; i < Platforms.size(); ++i)
    {
        if (Platforms[i] != platform)
            continue;

        b2DestroyBody(platform->body);

        delete Platforms[i];
        Platforms[i] = Platforms.back();
        Platforms.pop_back();

        return;
    }
}

void ClearPlatforms()
{
    for (size_t i = 0; i < Platforms.size(); ++i)
    {
        b2DestroyBody(Platforms[i]->body);
        delete Platforms[i];
    }

    Platforms.clear();
}
