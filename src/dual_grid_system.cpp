#include "dual_grid_system.h"
#include "settings.h"

Grid::Grid() : width(DEFAULT_GRID_SIZE), height(DEFAULT_GRID_SIZE), tiles(new bool[DEFAULT_GRID_SIZE * DEFAULT_GRID_SIZE]), tileValues(new TileValue[(DEFAULT_GRID_SIZE + 1) * (DEFAULT_GRID_SIZE + 1)]), scale(DEFAULT_GRID_SCALE), offset({0, 0}) {}

Grid::Grid(Grid &other) : width(other.width), height(other.height), tiles(new bool[other.width * other.height]), tileValues(new TileValue[(other.width + 1) * (other.height + 1)])
{
    for (int i = 0; i < other.width * other.height; ++i)
    {
        tiles[i] = other.tiles[i];
        tileValues[i] = other.tileValues[i];
    }

    for (int i = other.width * other.height; i < (other.width + 1) * (other.height + 1); ++i)
        tileValues[i] = other.tileValues[i];
}

Grid::Grid(int width, int height) : width(width), height(height), tiles(new bool[width * height]), tileValues(new TileValue[width * height]), scale(DEFAULT_GRID_SCALE), offset({0, 0}) {}

Grid::~Grid()
{
    delete[] tiles;
    delete[] tileValues;
}

void Grid::LoadTilesFromString(const char *data, int dataSize, char positiveValue)
{
    for (int i = 0; i < width * height; ++i)
    {
        if (i >= dataSize)
        {
            tiles[i] = false;
        }

        tiles[i] = ((*data) == positiveValue);
        ++data;
    }

    this->SetTileValues();
}

void Grid::DrawTiles()
{
    for (int i = 0; i < (width + 1) * (height + 1); ++i)
    {
        if (tileValues[i] == 0) continue;

        Rectangle source = {(tileValues[i] % 4) * TILE_SIZE, (tileValues[i] / 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE}, 
            dest = {(i % (width + 1)) * TILE_SIZE + (offset.x - 0.5f) * PIXELS_PER_UNIT, (i / (width + 1)) * TILE_SIZE + (offset.y - 0.5f) * PIXELS_PER_UNIT, TILE_SIZE * scale, TILE_SIZE * scale};

        DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
    }
}

bool Grid::GetTile(int x, int y)
{
    if (x < 0 || y < 0 || x >= width || y >= height)
    {
        //TraceLog(LOG_WARNING, "[Grid] Outside boundaries!");
        return false;
    }

    return tiles[y * width + x];
}

void Grid::SetTile(int x, int y)
{
    if (GetTile(x, y))
        return;

    tiles[y * width + x] = true;

    tileValues[y * width + x] += 8;
    tileValues[y * width + x + 1] += 4;
    tileValues[(y + 1) * width + x] += 2;
    tileValues[(y + 1) * width + x + 1] += 1;
}

void Grid::RemoveTile(int x, int y)
{
    if (!GetTile(x, y))
        return;

    tiles[y * width + x] = false;

    tileValues[y * width + x] -= 8;
    tileValues[y * width + x + 1] -= 4;
    tileValues[(y + 1) * width + x] -= 2;
    tileValues[(y + 1) * width + x + 1] -= 1;
}

void Grid::SetScale(float newScale)
{
    scale = newScale;
}

void Grid::SetOffset(Vector2 newOffset)
{
    offset = newOffset;
}

Vector2 Grid::GetOffset()
{
    return offset;
}

void Grid::SetTileValues()
{
    for (int i = 0; i < (width + 1) * (height + 1); ++i)
    {
        tileValues[i] = 0;
    }

    for (int i = 0; i < (width + 1) * (height + 1); ++i)
    {
        if (!GetTile(i % (width + 1), i / (height + 1))) continue;

        tileValues[i] |= 8;
        tileValues[i + 1] |= 4;
        tileValues[i + width + 1] |= 2;
        tileValues[i + width + 2] |= 1;
    }
}

Texture2D GenerateGridTemplate()
{
    Image img = GenImageColor(8, 8, RED);

    for (int i = 0; i < 16; ++i)
    {
        if (i & 1)
            ImageDrawPixel(&img, (i % 4) * 2, (i / 4) * 2, GREEN);
        if (i & 2)
            ImageDrawPixel(&img, (i % 4) * 2 + 1, (i / 4) * 2, GREEN);
        if (i & 4)
            ImageDrawPixel(&img, (i % 4) * 2, (i / 4) * 2 + 1, GREEN);
        if (i & 8)
            ImageDrawPixel(&img, (i % 4) * 2 + 1, (i / 4) * 2 + 1, GREEN);
    }

    ImageResizeNN(&img, 4 * TILE_SIZE, 4 * TILE_SIZE);

    Texture2D texture = LoadTextureFromImage(img);

    ExportImage(img, TextFormat("%s/grid_texture_template.png", GetWorkingDirectory()));

    UnloadImage(img);

    return texture;
}
