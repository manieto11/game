#include "materials.h"

Material PlayerMaterial;

void LoadGameMaterials()
{
    PlayerMaterial = LoadMaterialDefault();
    
}

void UnloadGameMaterials()
{
    UnloadMaterial(PlayerMaterial);
}
