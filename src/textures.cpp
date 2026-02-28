#include "textures.h"
#include "settings.h"

Texture2D DefaultTexture;
Texture2D DefaultGridTexture;

void LoadGameTextures()
{
    Image image = GenImageChecked(PIXELS_PER_UNIT, PIXELS_PER_UNIT, PIXELS_PER_UNIT / 2, PIXELS_PER_UNIT / 2, MAGENTA, BLACK);
    if (!IsImageValid(image))
        TraceLog(LOG_WARNING, "Image not valid!");
    DefaultTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void UnloadGameTextures()
{
    UnloadTexture(DefaultTexture);
}
