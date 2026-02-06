#include "textures.h"

void LoadGameTextures()
{
    Image image = GenImageChecked(32, 32, 2, 2, MAGENTA, BLACK);
    DefaultTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void UnloadGameTextures()
{
    UnloadTexture(DefaultTexture);
}
