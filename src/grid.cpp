#include "grid.h"
#include "game.h"
#include "settings.h"
#include <algorithm>
#include <set>

std::vector<std::unique_ptr<Grid>> Grids;

GridCell::GridCell() : texture(nullptr), isActive(false)
{
    textureSource = {0.0f, 0.0f, 0.0f, 0.0f};
}

Grid::Grid(b2Vec2 cellSize, int gridWidth, int gridHeight, bool hasCollision)
    : cellSize(cellSize), gridWidth(gridWidth), gridHeight(gridHeight), hasCollision(hasCollision)
{
}

Grid::~Grid()
{
    Clear();
}

void Grid::SetCell(int x, int y, bool active, Texture2D* texture)
{
    // Validate grid bounds
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        return;

    auto key = std::make_pair(x, y);
    auto it = cells.find(key);

    if (!active)
    {
        // Remove cell if it exists
        if (it != cells.end())
        {
            cells.erase(it);
        }
        // Rebuild colliders after removal
        if (hasCollision)
        {
            RebuildColliders();
        }
        return;
    }

    GridCell newCell;
    newCell.isActive = true;
    newCell.texture = texture;

    // Replace or insert
    if (it != cells.end())
    {
        it->second = newCell;
    }
    else
    {
        cells[key] = newCell;
    }

    // Rebuild colliders after adding/changing
    if (hasCollision)
    {
        RebuildColliders();
    }
}

GridCell* Grid::GetCell(int x, int y)
{
    auto it = cells.find(std::make_pair(x, y));
    if (it == cells.end() || !it->second.isActive)
        return nullptr;
    return &it->second;
}

void Grid::Clear()
{
    // Destroy all collider bodies
    for (auto& collider : colliders)
    {
        b2DestroyBody(collider.body);
    }
    colliders.clear();
    cells.clear();
}

b2Vec2 Grid::GetWorldPosition(int gridX, int gridY) const
{
    return {gridX * cellSize.x + cellSize.x / 2.0f, gridY * cellSize.y + cellSize.y / 2.0f};
}

void Grid::GetGridPosition(b2Vec2 worldPos, int& outX, int& outY) const
{
    outX = static_cast<int>(worldPos.x / cellSize.x);
    outY = static_cast<int>(worldPos.y / cellSize.y);
}

void Grid::RebuildColliders()
{
    // Destroy old colliders
    for (auto& collider : colliders)
    {
        b2DestroyBody(collider.body);
    }
    colliders.clear();

    if (!hasCollision)
        return;

    // Greedy rectangle merging algorithm
    MergeColliders();
}

void Grid::MergeColliders()
{
    // Track which cells have been covered
    std::set<std::pair<int, int>> covered;

    // Iterate through all cells
    for (int y = 0; y < gridHeight; ++y)
    {
        for (int x = 0; x < gridWidth; ++x)
        {
            auto key = std::make_pair(x, y);

            // Skip if already covered or not active
            if (covered.count(key) || cells.find(key) == cells.end() || !cells[key].isActive)
                continue;

            // Start a new rectangle from this cell
            int rectX = x;
            int rectY = y;
            int rectWidth = 1;
            int rectHeight = 1;

            // Expand horizontally as far as possible
            while (rectX + rectWidth < gridWidth)
            {
                auto checkKey = std::make_pair(rectX + rectWidth, rectY);
                if (cells.find(checkKey) == cells.end() || !cells[checkKey].isActive || covered.count(checkKey))
                    break;
                rectWidth++;
            }

            // Expand vertically as far as possible with current width
            while (rectY + rectHeight < gridHeight)
            {
                bool canExpand = true;
                for (int cx = rectX; cx < rectX + rectWidth; ++cx)
                {
                    auto checkKey = std::make_pair(cx, rectY + rectHeight);
                    if (cells.find(checkKey) == cells.end() || !cells[checkKey].isActive || covered.count(checkKey))
                    {
                        canExpand = false;
                        break;
                    }
                }
                if (!canExpand)
                    break;
                rectHeight++;
            }

            // Mark all cells in this rectangle as covered
            for (int cy = rectY; cy < rectY + rectHeight; ++cy)
            {
                for (int cx = rectX; cx < rectX + rectWidth; ++cx)
                {
                    covered.insert(std::make_pair(cx, cy));
                }
            }

            // Create collider for this rectangle
            ColliderRect collider;
            collider.gridX = rectX;
            collider.gridY = rectY;
            collider.width = rectWidth;
            collider.height = rectHeight;

            // Calculate world position (center of rectangle)
            b2Vec2 worldPos = {
                (rectX + rectWidth * 0.5f) * cellSize.x,
                (rectY + rectHeight * 0.5f) * cellSize.y
            };

            b2BodyDef bodyDef = b2DefaultBodyDef();
            bodyDef.position = worldPos;

            collider.body = b2CreateBody(MainWorld, &bodyDef);

            // Create polygon shape for this rectangle
            float halfWidth = (rectWidth * cellSize.x) / 2.0f;
            float halfHeight = (rectHeight * cellSize.y) / 2.0f;
            b2Polygon cellBox = b2MakeBox(halfWidth, halfHeight);
            
            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.material.friction = 0.0f;
            b2CreatePolygonShape(collider.body, &shapeDef, &cellBox);

            colliders.push_back(collider);
        }
    }
}

void Grid::Draw() const
{
    for (const auto& pair : cells)
    {
        if (!pair.second.isActive)
            continue;

        int gridX = pair.first.first;
        int gridY = pair.first.second;
        const GridCell& cell = pair.second;

        b2Vec2 worldPos = GetWorldPosition(gridX, gridY);
        Vector2 screenPos = {worldPos.x * PIXELS_PER_UNIT, -worldPos.y * PIXELS_PER_UNIT};

        Rectangle destRect = {
            screenPos.x - (cellSize.x * PIXELS_PER_UNIT) / 2.0f,
            screenPos.y - (cellSize.y * PIXELS_PER_UNIT) / 2.0f,
            cellSize.x * PIXELS_PER_UNIT,
            cellSize.y * PIXELS_PER_UNIT
        };

        if (cell.texture != nullptr)
        {
            DrawTexturePro(*cell.texture, cell.textureSource, destRect, {0, 0}, 0.0f, WHITE);
        }
        else
        {
            // Draw placeholder rect
            DrawRectangleRec(destRect, hasCollision ? GRAY : LIGHTGRAY);
        }
    }
}

void Grid::DrawDebug() const
{
    if (hasCollision)
    {
        // Draw merged collider rectangles
        for (const auto& collider : colliders)
        {
            b2Vec2 colliderWorldPos = {
                (collider.gridX + collider.width * 0.5f) * cellSize.x,
                (collider.gridY + collider.height * 0.5f) * cellSize.y
            };
            Vector2 screenPos = {colliderWorldPos.x * PIXELS_PER_UNIT, -colliderWorldPos.y * PIXELS_PER_UNIT};

            float halfW = (collider.width * cellSize.x * PIXELS_PER_UNIT) / 2.0f;
            float halfH = (collider.height * cellSize.y * PIXELS_PER_UNIT) / 2.0f;

            DrawLineV({screenPos.x - halfW, screenPos.y - halfH}, {screenPos.x + halfW, screenPos.y - halfH}, DARKBLUE);
            DrawLineV({screenPos.x + halfW, screenPos.y - halfH}, {screenPos.x + halfW, screenPos.y + halfH}, DARKBLUE);
            DrawLineV({screenPos.x + halfW, screenPos.y + halfH}, {screenPos.x - halfW, screenPos.y + halfH}, DARKBLUE);
            DrawLineV({screenPos.x - halfW, screenPos.y + halfH}, {screenPos.x - halfW, screenPos.y - halfH}, DARKBLUE);
        }
    }
    else
    {
        // Draw individual prop cells
        for (const auto& pair : cells)
        {
            if (!pair.second.isActive)
                continue;

            int gridX = pair.first.first;
            int gridY = pair.first.second;

            b2Vec2 worldPos = GetWorldPosition(gridX, gridY);
            Vector2 screenPos = {worldPos.x * PIXELS_PER_UNIT, -worldPos.y * PIXELS_PER_UNIT};

            float halfW = (cellSize.x * PIXELS_PER_UNIT) / 2.0f;
            float halfH = (cellSize.y * PIXELS_PER_UNIT) / 2.0f;

            DrawLineV({screenPos.x - halfW, screenPos.y - halfH}, {screenPos.x + halfW, screenPos.y - halfH}, GREEN);
            DrawLineV({screenPos.x + halfW, screenPos.y - halfH}, {screenPos.x + halfW, screenPos.y + halfH}, GREEN);
            DrawLineV({screenPos.x + halfW, screenPos.y + halfH}, {screenPos.x - halfW, screenPos.y + halfH}, GREEN);
            DrawLineV({screenPos.x - halfW, screenPos.y + halfH}, {screenPos.x - halfW, screenPos.y - halfH}, GREEN);
        }
    }
}


Grid* CreateGrid(b2Vec2 cellSize, int gridWidth, int gridHeight, bool hasCollision)
{
    auto grid = std::make_unique<Grid>(cellSize, gridWidth, gridHeight, hasCollision);
    Grid* gridPointer = grid.get();
    
    if (AddGrid(std::move(grid)))
    {
        return gridPointer;
    }
    
    return nullptr;
}

bool AddGrid(std::unique_ptr<Grid> grid)
{
    Grids.push_back(std::move(grid));
    return true;
}

void RemoveGrid(Grid* grid)
{
    auto it = std::find_if(Grids.begin(), Grids.end(),
        [grid](const std::unique_ptr<Grid>& g) { return g.get() == grid; });
    
    if (it != Grids.end())
    {
        Grids.erase(it);
    }
}

void ClearGrids()
{
    for (auto& grid : Grids)
    {
        grid->Clear();
    }
    Grids.clear();
}
