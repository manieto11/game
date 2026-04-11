#include "game.h"
#include "player.h"
#include "settings.h"
#include "textures.h"
#include <memory>
#include <utility>

Camera MainCamera;
b2WorldId MainWorld;
std::vector<std::unique_ptr<Entity>> Entities;
std::vector<std::unique_ptr<Platform>> Platforms;

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
    for (const auto& entity : Entities)
    {
        if (entity.get() == PlayerEntity)
            continue;

        DrawEntity(entity.get());
    }

    for (const auto& platform : Platforms)
    {
        DrawPlatform(platform.get());
    }

    DrawPlayer();
}

void DrawDebug()
{
    for (const auto& entity : Entities)
    {
        DrawEntityBorder(entity.get());
    }

    for (const auto& platform : Platforms)
    {
        DrawPlatformBorders(platform.get());
    }

    DrawPlayerDebug();
}

void FinishGame()
{
    ClearEntities();
    ClearPlatforms();

    b2DestroyWorld(MainWorld);
    
    UnloadGameTextures();
}

Entity *CreateEntity()
{
    auto entity = std::make_unique<Entity>();

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

    Entity* entityPointer = entity.get();
    if (!AddEntity(std::move(entity)))
    {
        b2DestroyBody(entityPointer->body);
        return nullptr;
    }

    return entityPointer;
}

bool AddEntity(std::unique_ptr<Entity> entity)
{
    if (Entities.size() >= MAX_ENTITIES)
    {
        TraceLog(LOG_WARNING, "Not enough space for more entities!");
        return false;
    }

    Entities.push_back(std::move(entity));

    return true;
}

void RemoveEntity(Entity *entity)
{
    if (entity == nullptr)
        return;

    for (size_t i = 0; i < Entities.size(); ++i)
    {
        if (Entities[i].get() != entity)
            continue;

        b2DestroyBody(entity->body);

        if (i != Entities.size() - 1)
            Entities[i] = std::move(Entities.back());
        Entities.pop_back();

        break;
    }
}

void ClearEntities()
{
    for (const auto& entity : Entities)
    {
        b2DestroyBody(entity->body);
    }

    Entities.clear();
}

Platform *CreatePlatform()
{
    auto platform = std::make_unique<Platform>();

    b2BodyDef bodyDef = b2DefaultBodyDef();

    platform->body = b2CreateBody(MainWorld, &bodyDef);

    b2Polygon groundBox = b2MakeBox(0.5f, 0.5f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    groundShapeDef.material.friction = 0.0f;
    b2CreatePolygonShape(platform->body, &groundShapeDef, &groundBox);

    Platform* platformPointer = platform.get();
    if (!AddPlatform(std::move(platform)))
    {
        b2DestroyBody(platformPointer->body);
        return nullptr;
    }

    return platformPointer;
}

Platform *CreatePlatform(b2Vec2 size, b2Vec2 position)
{
    auto platform = std::make_unique<Platform>(size);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.position = {position.x, position.y};

    platform->body = b2CreateBody(MainWorld, &bodyDef);

    b2Polygon groundBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(platform->body, &groundShapeDef, &groundBox);

    Platform* platformPointer = platform.get();
    if (!AddPlatform(std::move(platform)))
    {
        b2DestroyBody(platformPointer->body);
        return nullptr;
    }

    return platformPointer;
}

bool AddPlatform(std::unique_ptr<Platform> platform)
{
    if (Platforms.size() >= MAX_PLATFORMS)
    {
        TraceLog(LOG_WARNING, "Not enough space for more platforms!");
        return false;
    }

    Platforms.push_back(std::move(platform));

    return true;
}

void RemovePlatform(Platform *platform)
{
    for (size_t i = 0; i < Platforms.size(); ++i)
    {
        if (Platforms[i].get() != platform)
            continue;

        b2DestroyBody(platform->body);

        if (i != Platforms.size() - 1)
            Platforms[i] = std::move(Platforms.back());
        Platforms.pop_back();

        return;
    }
}

void ClearPlatforms()
{
    for (const auto& platform : Platforms)
    {
        b2DestroyBody(platform->body);
    }

    Platforms.clear();
}
