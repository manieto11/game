#include "game.h"
#include "settings.h"
#include "string.h"
#include "textures.h"
#include "dual_grid_system.h"

Camera2D *MainCamera = nullptr;
Player *MainPlayer = nullptr;
GameObject *GameObjects[MAX_GAME_OBJECTS] = {nullptr};
LevelManager *MainLevelManager = nullptr;
Texture2D LevelTileset;
Collider *GameColliders[MAX_COLLIDERS] = {nullptr};
int ColliderCount = 0;

void InitGame()
{
    LoadSettings();
    LoadGameTextures();

    MainCamera = new Camera2D();

    MainCamera->offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    MainCamera->rotation = 0;
    MainCamera->target = {0, 0};
    MainCamera->zoom = scaleMultiplier;

    // Initialize level system
    MainLevelManager = new LevelManager();
    LevelTileset = GenerateGridTemplate();

    std::string levelData = "#####"
                            "#   #"
                            "#   #"
                            "#   #"
                            "#####";

    for (int i = 0; i < 3; ++i)
    {
        Level *level = new Level();
        
        if (level->LoadFromString(levelData, 5, 5, '#'))
        {
            level->SetTexture(LevelTileset);
            level->SetOffset({-2.5f, 2.5f});
            level->SetScale(1.0f);
            level->SetPlayerSpawn({1.0f, 2.0f});
            MainLevelManager->AddLevel(level);
            
            TraceLog(LOG_INFO, "Loaded level %d: %dx%d", 
                     i+1, level->GetWidth(), level->GetHeight());
        }
        else
        {
            TraceLog(LOG_WARNING, "Failed to load level");
            delete level;
        }
    }

    // Initialize player
    Player *player = new Player();
    GameObjects[0] = player;
    player->Initialize();

    AddCollider(player->collider);

    // Spawn player at first level's spawn point
    Level *firstLevel = MainLevelManager->GetCurrentLevel();
    if (firstLevel != nullptr)
    {
        Vector2 spawn = firstLevel->GetPlayerSpawn();
        player->transform->position = spawn;
        TraceLog(LOG_INFO, "Player spawned at (%.1f, %.1f)", spawn.x, spawn.y);
        AddCollider(firstLevel->GetGridCollider());
    }

    MainPlayer = player;
}

void UpdateGame()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; ++i)
    {
        if (GameObjects[i] == nullptr)
            continue;

        GameObjects[i]->Update();
    }
}

void FixedUpdateGame()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; ++i)
    {
        if (GameObjects[i] == nullptr)
            continue;

        GameObjects[i]->FixedUpdate();
    }
}

void DrawGame()
{
    // Draw current level first
    Level *currentLevel = MainLevelManager->GetCurrentLevel();
    if (currentLevel != nullptr)
    {
        currentLevel->Draw();
    }

    // Draw game objects
    for (int i = 0; i < MAX_GAME_OBJECTS; ++i)
    {
        if (GameObjects[i] == nullptr)
            continue;

        GameObjects[i]->Draw();
    }
}

void FinishGame()
{
    delete MainCamera;

    // Clean up level system
    if (MainLevelManager != nullptr)
    {
        delete MainLevelManager;
        MainLevelManager = nullptr;
    }

    UnloadTexture(LevelTileset);

    for (int i = 0; i < MAX_GAME_OBJECTS; ++i)
    {
        if (GameObjects[i] == nullptr)
            continue;

        delete GameObjects[i];
    }
}

bool AddCollider(Collider *collider)
{
    if (ColliderCount >= MAX_COLLIDERS)
    {
        TraceLog(LOG_WARNING, "Couldn't add collider!");
        return false;
    }

    GameColliders[ColliderCount] = collider;
    ColliderCount++;

    return true;
}

void RemoveCollider(Collider *collider)
{
    for (int i = 0; i < ColliderCount; i++)
    {
        if (collider != GameColliders[i]) continue;
        
        ColliderCount--;
        GameColliders[i] = GameColliders[ColliderCount];
        return;
    }
    
    TraceLog(LOG_WARNING, "Couldn't remove collider!");
}

void RemoveCollider(int index)
{
    if (index >= ColliderCount) 
    {
        TraceLog(LOG_WARNING, "Couldn't remove collider!");
        return;
    }
        
    ColliderCount--;
    GameColliders[index] = GameColliders[ColliderCount];
}

void ClearColliders()
{
    ColliderCount = 0;
}

bool NextLevel()
{
    RemoveCollider(MainLevelManager->GetCurrentLevel()->GetGridCollider());

    if (!MainLevelManager->NextLevel())
    {
        return false;
    }

    AddCollider(MainLevelManager->GetCurrentLevel()->GetGridCollider());

    return true;
}
