#include "textures.h"
#include "settings.h"

Texture2D DefaultTexture, BackgroundTexture, IronBeamTexture, PlayerTexture;

void LoadGameTextures()
{
    Image image = GenImageChecked(2, 2, 1, 1, MAGENTA, BLACK);
    if (!IsImageValid(image))
        TraceLog(LOG_WARNING, "Image not valid!");
    DefaultTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    BackgroundTexture = LoadTexture("../../textures/background.png");
    IronBeamTexture = LoadTexture("../../textures/iron_beam.png");
    PlayerTexture = LoadTexture("../../textures/main_character.png");
}

void UnloadGameTextures()
{
    UnloadTexture(DefaultTexture);

    UnloadTexture(BackgroundTexture);
    UnloadTexture(IronBeamTexture);
    UnloadTexture(PlayerTexture);
}
