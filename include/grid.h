#ifndef GRID_H
#define GRID_H

#include "box2d/box2d.h"
#include "raylib.h"
#include <vector>
#include <map>
#include <memory>

// A single grid cell
struct GridCell
{
    Texture2D* texture; // Optional texture
    Rectangle textureSource;
    bool isActive;      // Whether this cell exists

    GridCell();
};

// A merged collision rectangle containing multiple cells
struct ColliderRect
{
    b2BodyId body;      // Physics body
    int gridX;          // Top-left grid position
    int gridY;
    int width;          // Size in grid cells
    int height;
};

// Grid system for managing tileable content
class Grid
{
public:
    Grid(b2Vec2 cellSize, int gridWidth, int gridHeight, bool hasCollision);
    ~Grid();

    // Add or update a cell at grid position
    void SetCell(int x, int y, bool active, Texture2D* texture = nullptr);
    
    // Get cell at grid position
    GridCell* GetCell(int x, int y);
    
    // Remove all cells
    void Clear();

    // Get world position from grid position
    b2Vec2 GetWorldPosition(int gridX, int gridY) const;
    
    // Get grid position from world position
    void GetGridPosition(b2Vec2 worldPos, int& outX, int& outY) const;

    // Rebuild collision rectangles from active cells
    void RebuildColliders();

    // Render the grid
    void Draw() const;
    
    // Render debug borders
    void DrawDebug() const;

    // Properties
    b2Vec2 cellSize;
    int gridWidth;
    int gridHeight;
    bool hasCollision;

private:
    std::map<std::pair<int, int>, GridCell> cells;
    std::vector<ColliderRect> colliders;  // Merged collision rectangles
    
    // Algorithm to merge adjacent cells into rectangles
    void MergeColliders();
};

// Global grid management
extern std::vector<std::unique_ptr<Grid>> Grids;

#endif
