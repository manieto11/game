#ifndef DUAL_GRID_SYSTEM_H
#define DUAL_GRID_SYSTEM_H

#include "raylib.h"

typedef unsigned char TileValue;

class Grid
{
private:
    int height, width;
    float scale;
    bool *tiles;
    TileValue *tileValues;
    Vector2 offset;

public:
    Texture2D texture;

    Grid();
    Grid(Grid &other);
    Grid(int width, int height);
    ~Grid();

    void LoadTilesFromString(const char *data, int dataSize, char positiveValue);
    void DrawTiles();

    bool GetTile(int x, int y);
    void SetTile(int x, int y);
    void RemoveTile(int x, int y);

    void SetScale(float newScale);

    void SetTileValues();

    void SetOffset(Vector2 offset);
    Vector2 GetOffset();
};

Texture2D GenerateGridTemplate();

#endif
