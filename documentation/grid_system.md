# Grid System Documentation

## Overview

The Grid system provides an efficient tile-based level representation using a dual-grid approach with marching squares for rendering. It's designed for platformer levels with solid/empty tiles.

## Grid Class

### Class Structure

```cpp
class Grid
{
private:
    int height, width;          // Grid dimensions
    float scale;                // Visual scale multiplier
    bool *tiles;                // Tile solidity array
    TileValue *tileValues;      // Marching squares values
    Vector2 offset;             // World position offset

public:
    Texture2D texture;          // Tileset texture

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
};
```

## Creating a Grid

### Basic Grid Creation

```cpp
// Create a 64x64 tile grid
Grid levelGrid(64, 64);

// Set texture
levelGrid.texture = GenerateGridTemplate();  // Or load custom texture

// Set scale
levelGrid.SetScale(1.0f);

// Set world offset
levelGrid.SetOffset({0, 0});
```

### Loading Level Data

```cpp
// From string (common for simple levels)
const char* levelData = 
    "####################"
    "#..................#"
    "#........#.........#"
    "#.......###........#"
    "#..................#"
    "####################";

grid.LoadTilesFromString(levelData, strlen(levelData), '#');

// From file
std::ifstream file("level.txt");
std::string content((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
grid.LoadTilesFromString(content.c_str(), content.size(), '#');
```

## Tile Operations

### Getting Tile State

```cpp
// Check if tile is solid
bool IsSolid(int x, int y)
{
    return grid.GetTile(x, y);
}

// GetTile returns false for out-of-bounds coordinates
bool outOfBounds = grid.GetTile(-1, 0);  // Returns false
```

### Setting Tiles

```cpp
// Add a solid tile
grid.SetTile(10, 5);

// Does nothing if tile is already set
grid.SetTile(10, 5);  // No effect

// Remove a tile
grid.RemoveTile(10, 5);

// Does nothing if tile is already empty
grid.RemoveTile(10, 5);  // No effect
```

### Dynamic Tile Modification

```cpp
// Example: Destructible terrain
void DestroyTileAt(Grid *grid, Vector2 worldPos)
{
    int tileX = (int)worldPos.x;
    int tileY = (int)worldPos.y;
    
    if (grid->GetTile(tileX, tileY))
    {
        grid->RemoveTile(tileX, tileY);
        // Spawn particles, play sound, etc.
    }
}
```

## Rendering System

### Marching Squares Algorithm

The grid uses marching squares for smooth tile rendering. Each vertex between tiles has a value (0-15) representing which adjacent tiles are solid:

```
8 --- 4
|     |
|     |
2 --- 1
```

- Bit 0 (1): Bottom-right tile
- Bit 1 (2): Bottom-left tile  
- Bit 2 (4): Top-right tile
- Bit 3 (8): Top-left tile

### Texture Layout

The grid texture must be a 4x4 grid of tiles (16 tiles total):

```
[0 ][1 ][2 ][3 ]
[4 ][5 ][6 ][7 ]
[8 ][9 ][10][11]
[12][13][14][15]
```

Each tile corresponds to a marching squares configuration.

### Generating Default Texture

```cpp
Texture2D GenerateGridTemplate()
{
    // Creates a simple test texture with green pixels
    // showing tile configurations
    Image img = GenImageColor(8, 8, RED);
    
    // Mark corners based on bit values
    for (int i = 0; i < 16; ++i)
    {
        if (i & 1) ImageDrawPixel(&img, ...);  // Bottom-right
        if (i & 2) ImageDrawPixel(&img, ...);  // Bottom-left
        if (i & 4) ImageDrawPixel(&img, ...);  // Top-right
        if (i & 8) ImageDrawPixel(&img, ...);  // Top-left
    }
    
    ImageResizeNN(&img, 4 * TILE_SIZE, 4 * TILE_SIZE);
    return LoadTextureFromImage(img);
}
```

### Custom Tileset

Create a custom tileset texture (128x128 pixels for 32px tiles):

```cpp
// Load custom tileset
Texture2D tileset = LoadTexture("tileset.png");
grid.texture = tileset;

// Ensure texture follows marching squares layout
// Tile 0 (0000): Empty
// Tile 1 (0001): Bottom-right corner
// Tile 2 (0010): Bottom-left corner
// Tile 3 (0011): Bottom edge
// ... etc
```

## Coordinate Systems

### Grid Space

- Integer coordinates representing tile positions
- Origin (0, 0) is bottom-left
- X increases right, Y increases up

### World Space

- Float coordinates representing game world positions
- Grid tile (x, y) occupies world space [x, x+1] × [y, y+1]
- Use offset to position grid in world

### Screen Space

- Pixel coordinates for rendering
- Calculated as: `screenPos = (gridPos * TILE_SIZE * scale) + (offset * PIXELS_PER_UNIT)`

### Conversion Example

```cpp
// World to grid
Vector2 WorldToGrid(Vector2 worldPos)
{
    return {(float)(int)worldPos.x, (float)(int)worldPos.y};
}

// Grid to world (center of tile)
Vector2 GridToWorld(int gridX, int gridY)
{
    return {gridX + 0.5f, gridY + 0.5f};
}
```

## Performance Optimization

### Rendering Optimization

The grid only renders visible tiles:

```cpp
void DrawTiles()
{
    for (int i = 0; i < (width + 1) * (height + 1); ++i)
    {
        if (tileValues[i] == 0) continue;  // Skip empty tiles
        
        // Render only non-zero tiles
        DrawTexturePro(...);
    }
}
```

### Memory Usage

```cpp
// For a 128x128 grid:
// Tiles: 128 × 128 × 1 byte = 16 KB
// TileValues: 129 × 129 × 1 byte = 16.6 KB
// Total: ~33 KB per grid
```

### Best Practices

1. **Reasonable Grid Sizes**: Keep grids under 256x256 for good performance
2. **Reuse Textures**: Use one texture for all grid instances
3. **Avoid Frequent Updates**: SetTile/RemoveTile updates 4 TileValues each time
4. **Spatial Queries**: For many collision checks, consider spatial partitioning

## Example: Complete Level Setup

```cpp
class Level
{
private:
    Grid grid;
    GridCollider collider;
    
public:
    Level(const char* mapFile)
        : grid(128, 64),
          collider(&grid)
    {
        // Load texture
        grid.texture = LoadTexture("platformer_tiles.png");
        
        // Load level data
        std::ifstream file(mapFile);
        std::string data((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
        grid.LoadTilesFromString(data.c_str(), data.size(), '#');
        
        // Position in world
        grid.SetOffset({0, 0});
        grid.SetScale(1.0f);
    }
    
    void Draw()
    {
        grid.DrawTiles();
    }
    
    bool CheckCollision(Vector2 position)
    {
        return collider.Colliding(position);
    }
    
    void DestroyTile(Vector2 worldPos)
    {
        int x = (int)worldPos.x;
        int y = (int)worldPos.y;
        grid.RemoveTile(x, y);
    }
};
```

## Common Issues

**Issue**: "Tiles not rendering"
- Ensure texture is loaded and valid
- Check that LoadTilesFromString was called
- Verify SetTileValues() was called (done automatically by LoadTilesFromString)

**Issue**: "Tiles in wrong position"  
- Check offset is set correctly
- Verify coordinate system (Y-up vs Y-down)
- Ensure scale matches expected size

**Issue**: "Collision not working"
- GridCollider converts world coords to grid coords by truncating
- Ensure world position matches grid coordinates
- Check tile exists with GetTile()

## Advanced Features

### Tile Metadata

Extend the system to support different tile types:

```cpp
enum TileType { EMPTY, SOLID, PLATFORM, LADDER, SPIKE };

class ExtendedGrid : public Grid
{
private:
    TileType *tileTypes;
    
public:
    void SetTileType(int x, int y, TileType type)
    {
        if (type == EMPTY)
            RemoveTile(x, y);
        else
            SetTile(x, y);
        
        tileTypes[y * width + x] = type;
    }
    
    TileType GetTileType(int x, int y)
    {
        if (!GetTile(x, y))
            return EMPTY;
        return tileTypes[y * width + x];
    }
};
```

### Animated Tiles

Add frame animation support:

```cpp
class AnimatedGrid
{
private:
    Grid grid;
    float animTime;
    int currentFrame;
    
public:
    void Update(float deltaTime)
    {
        animTime += deltaTime;
        if (animTime >= 0.2f)  // 5 FPS
        {
            currentFrame = (currentFrame + 1) % 4;
            animTime = 0;
            // Update texture coordinates based on frame
        }
    }
};
```

## Future Enhancements

- [ ] Multiple tile layers (background, foreground)
- [ ] Tilemap chunking for huge levels
- [ ] Autotiling for easier level creation
- [ ] Slope/ramp collision support
- [ ] Tile property system (friction, damage, etc.)
- [ ] Level serialization/deserialization
