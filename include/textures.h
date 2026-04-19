#ifndef TEXTURES_H
#define TEXTURES_H

#include "raylib.h"

#pragma message ("Change loading system to embedded files!")

extern Texture2D DefaultTexture;

extern Texture2D BackgroundTexture;
extern Texture2D GroundAtlasTexture;
extern Texture2D IronBeamTexture;
extern Texture2D PlayerTexture;

// Load game textures. Call UnloadGameTextures before ending execution
void LoadGameTextures();
// Unload game textures
void UnloadGameTextures();

#endif