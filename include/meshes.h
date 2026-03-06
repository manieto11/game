#ifndef MESHES_H
#define MESHES_H

#include "raylib.h"

extern Mesh PlayerBodyMesh;
extern Mesh PlayerLegMesh;

// Load game meshes. Use UnloadGameMeshes before ending execution
void LoadGameMeshes();
// Unload game meshes
void UnloadGameMeshes();

#endif