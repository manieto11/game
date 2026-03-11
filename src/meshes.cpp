/*
#include "meshes.h"
#include "settings.h"

Mesh PlayerBodyMesh;
Mesh PlayerLegMesh;

float PlayerBodyVertices[] = {-0.35f * PIXELS_PER_UNIT, - 0.5f * PIXELS_PER_UNIT, 0.0f, -0.2f * PIXELS_PER_UNIT, 0.05f * PIXELS_PER_UNIT, 0.0f, 0.35f * PIXELS_PER_UNIT, - 0.5f * PIXELS_PER_UNIT, 0.0f, 0.2f * PIXELS_PER_UNIT, 0.05f * PIXELS_PER_UNIT, 0.0f}; //{0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
float PlayerLegVertices[] = {0.0f, -0.5f * PIXELS_PER_UNIT, 0.0f, -0.15f * PIXELS_PER_UNIT, -0.35f * PIXELS_PER_UNIT, 0.0f, 0.15f * PIXELS_PER_UNIT, -0.35f * PIXELS_PER_UNIT, 0.0f, 0.0f, 0.0f, 0.0f};

unsigned short Indices[] = {0, 1, 2, 3, 2, 1};

unsigned char VertexColors[] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

void LoadGameMeshes()
{
    PlayerBodyMesh.vertexCount = 4;
    PlayerBodyMesh.triangleCount = 2;
    PlayerBodyMesh.vertices = PlayerBodyVertices;
    PlayerBodyMesh.indices = Indices;
    PlayerBodyMesh.colors = VertexColors;

    UploadMesh(&PlayerBodyMesh, false);

    PlayerLegMesh.vertexCount = 4;
    PlayerLegMesh.triangleCount = 2;
    PlayerLegMesh.vertices = PlayerLegVertices;
    PlayerLegMesh.indices = Indices;
    PlayerLegMesh.colors = VertexColors;

    UploadMesh(&PlayerLegMesh, false);
}

void UnloadGameMeshes()
{
    UnloadMesh(PlayerBodyMesh);
    UnloadMesh(PlayerLegMesh);
}
*/