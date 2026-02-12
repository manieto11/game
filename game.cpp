#include "game.h"

Camera2D *MainCamera = nullptr;
Player *MainPlayer = nullptr;
GameObject *gameObjects[MAX_GAME_OBJECTS] = {nullptr};

void InitGame()
{
    LoadSettings();
    LoadGameTextures();

    MainCamera = new Camera2D();

    MainCamera->offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    MainCamera->rotation = 0;
    MainCamera->target = {0, 0};
    MainCamera->zoom = scaleMultiplier;

    Player *player = new Player();
    player->rigidBody.gravity = 0.0f;
    gameObjects[0] = player;
    player->Initialize();

    MainPlayer = player;
}

void UpdateGame()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; ++i) {
        if (gameObjects[i] == nullptr) continue;

        gameObjects[i]->Update();
    }
}

void FixedUpdateGame()
{
    for (int i = 0; i < MAX_GAME_OBJECTS; ++i) {
        if (gameObjects[i] == nullptr) continue;

        gameObjects[i]->FixedUpdate();
    }
}

void FinishGame()
{
    delete MainCamera;

    for (int i = 0; i < MAX_GAME_OBJECTS; ++i) {
        if (gameObjects[i] == nullptr) continue;

        delete gameObjects[i];
    }
}
