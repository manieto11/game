#include "meshes.h"

Mesh PlayerBodyMesh;
Mesh PlayerLegMesh;

float PlayerBodyVertices[] = {0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f}; //{-0.25f, 0.2f, 0.0f, 0.25f, 0.2f, 0.0f, -0.25f, -0.2f, 0.0f, 0.25f, -0.2f, 0.0f};
float PlayerLegVertices[] = {0.0f, 4.8f, 0.0f, -1.6f, 3.2f, 0.0f, 1.6f, 3.2f, 0.0f, 0.0f, -4.8f, 0.0f};

unsigned short PlayerBodyIndices[] = {1, 0, 3, 0, 2, 3};
unsigned short PlayerLegIndices[] = {0, 1, 2, 3, 2, 1};

unsigned char PlayerBodyColors[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

void LoadGameMeshes()
{
    PlayerBodyMesh.vertexCount = 4;
    PlayerBodyMesh.triangleCount = 2;
    PlayerBodyMesh.vertices = PlayerBodyVertices;
    PlayerBodyMesh.indices = PlayerBodyIndices;
    PlayerBodyMesh.colors = PlayerBodyColors;

    UploadMesh(&PlayerBodyMesh, false);
}

void UnloadGameMeshes()
{
    UnloadMesh(PlayerBodyMesh);
}
