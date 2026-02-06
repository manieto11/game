#include "game.h"
#include "settings.h"

Camera2D *MainCamera = nullptr;

void InitGame()
{
    MainCamera = new Camera2D();

    MainCamera->offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    MainCamera->rotation = 0;
    MainCamera->target = {0, 0};
    MainCamera->zoom = scaleMultiplier;
}

void FinishGame()
{
    delete MainCamera;
}
