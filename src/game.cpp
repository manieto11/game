#include "game.h"
#include "player.h"
#include "settings.h"
#include "textures.h"
#include <memory>
#include <utility>

Camera2D MainCamera;
b2WorldId MainWorld;
std::vector<std::unique_ptr<Entity>> Entities;

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

    MainCamera.target = {0.0f, 0.0f};
    MainCamera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    MainCamera.rotation = 0.0f;
    MainCamera.zoom = 1.0f;

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -GAME_GRAVITY};

    MainWorld = b2CreateWorld(&worldDef);

    PlayerEntity = CreateEntity(2.0f, 2.0f);

    // Create ground collision grid (1x1 cells, 20 wide, 10 tall)
    Grid* groundGrid = CreateGrid({1.0f, 1.0f}, 20, 10, true);

    groundGrid->SetTexture(&GroundAtlasTexture);
    
    // Populate ground level with collision tiles (y = 0)
    for (int x = 1; x < 19; ++x)
    {
        groundGrid->SetCell(x, 0, 7);
        groundGrid->SetCell(x, 9, 1);
    }

    groundGrid->SetCell(0, 0, 8);
    groundGrid->SetCell(19, 0, 8);

    for (int y = 1; y < 9; ++y)
    {
        groundGrid->SetCell(0, y, 4);
        groundGrid->SetCell(19, y, 4);
    }

    groundGrid->SetCell(0, 9, 0);
    groundGrid->SetCell(19, 9, 3);

    /*// Create prop grid (no collision)
    Grid *propGrid = CreateGrid({1.0f, 1.0f}, 20, 10, false);

    // Add some example prop tiles
    for (int x = 2; x < 5; ++x)
    {
        propGrid->SetCell(x, 3, true);
    }
    for (int x = 10; x < 13; ++x)
    {
        propGrid->SetCell(x, 4, true);
    }*/
}

void UpdateGame()
{
    UpdatePlayer();
    UpdateCameraFollowPlayer();
}

void FixedUpdateGame()
{
    b2World_Step(MainWorld, FIXED_DELTA_TIME, 4);
}

void DrawGame()
{
    // Draw grids (background to foreground)
    for (const auto& grid : Grids)
    {
        grid->Draw();
    }

    for (const auto& entity : Entities)
    {
        if (entity.get() == PlayerEntity)
            continue;

        DrawEntity(entity.get());
    }

    DrawPlayer();
}

void DrawDebug()
{
    for (const auto& grid : Grids)
    {
        grid->DrawDebug();
    }

    for (const auto& entity : Entities)
    {
        DrawEntityBorder(entity.get());
    }

    DrawPlayerDebug();
}

void FinishGame()
{
    SaveSettings();

    ClearEntities();
    ClearGrids();

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

Entity *CreateEntity(float x, float y)
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

    bodyDef.position = {x, y};

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

void UpdateCameraFollowPlayer()
{
    if (PlayerEntity == nullptr)
        return;

    b2Vec2 playerPos = b2Body_GetPosition(PlayerEntity->body);
    const Vector2 playerScreenPos = {playerPos.x * PIXELS_PER_UNIT, -playerPos.y * PIXELS_PER_UNIT};
    
    // Define dead zone - player can move this far before camera follows
    float deadzoneWidth = CAMERA_DEADZONE_WIDTH * PIXELS_PER_UNIT;
    float deadzoneHeight = deadzoneWidth * (static_cast<float>(GetScreenHeight()) / GetScreenWidth());
    
    // Calculate the bounds of the dead zone around the current camera target
    float deadzoneLeft = MainCamera.target.x - deadzoneWidth / 2.0f;
    float deadzoneRight = MainCamera.target.x + deadzoneWidth / 2.0f;
    float deadzoneTop = MainCamera.target.y - deadzoneHeight / 2.0f;
    float deadzoneBottom = MainCamera.target.y + deadzoneHeight / 2.0f;
    
    // Update camera target to follow player when outside deadzone
    if (playerScreenPos.x < deadzoneLeft)
    {
        MainCamera.target.x = playerScreenPos.x + deadzoneWidth / 2.0f;
    }
    else if (playerScreenPos.x > deadzoneRight)
    {
        MainCamera.target.x = playerScreenPos.x - deadzoneWidth / 2.0f;
    }
    
    if (playerScreenPos.y < deadzoneTop)
    {
        MainCamera.target.y = playerScreenPos.y + deadzoneHeight / 2.0f;
    }
    else if (playerScreenPos.y > deadzoneBottom)
    {
        MainCamera.target.y = playerScreenPos.y - deadzoneHeight / 2.0f;
    }
}
