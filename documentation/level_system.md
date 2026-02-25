# Level System Documentation

## Overview

The Level system provides a complete solution for creating, loading, managing, and transitioning between game levels. It integrates with the Grid system for tile-based collision and rendering, and supports metadata like spawn points and markers for special locations.

## Architecture

The level system consists of two main classes:

- **Level**: Represents a single game level with tiles, collision, spawn points, and markers
- **LevelManager**: Manages multiple levels and handles transitions between them

## Level Class

### Creating Levels

#### Method 1: Create Empty Level

```cpp
// Create a new empty 50x30 level
Level *level = new Level(50, 30, "Level 1");

// Manually set tiles
level->SetTile(10, 5);
level->SetTile(11, 5);
level->SetTile(12, 5);

// Set spawn point
level->SetPlayerSpawn({5.0f, 10.0f});
```

#### Method 2: Load from File

```cpp
Level *level = new Level();
level->LoadFromFile("levels/level1.txt", '#');

// Or pass the name in constructor
Level *level = new Level(50, 30, "Level 1");
level->LoadFromFile("levels/level1.txt");
```

#### Method 3: Load from String

```cpp
std::string levelData = 
    "##########"
    "#........#"
    "#..####..#"
    "#........#"
    "##########";

Level *level = new Level(10, 5, "Test Level");
level->LoadFromString(levelData, 10, 5, '#');
```

### Level File Format

Level files are simple text files with metadata headers and tile data.

#### Basic Format

```
@spawn X Y
@marker NAME X Y
[tile data]
```

#### Example Level File

```
@spawn 5 10
@marker goal 45 10
@marker checkpoint 20 15
##################################################
#................................................#
#................................................#
#..........####..............................####
#................................................#
#.....####...................................####
#................................................#
##################################################
```

#### Format Details

**Metadata Lines** (optional, at the top):
- `@spawn X Y` - Player spawn position (float coordinates)
- `@marker NAME X Y` - Named marker position (multiple allowed)

**Tile Data**:
- Each line represents one row of tiles
- `#` = solid tile (configurable character)
- `.` or space = empty tile
- Top line = highest Y coordinate (Y-axis points up)
- Automatically padded to create rectangular levels

### Using Levels

#### Basic Setup

```cpp
// Create and load level
Level *level = new Level(64, 32, "Forest Level");
level->LoadFromFile("levels/forest.txt");

// Set texture
Texture2D tileset = LoadTexture("tileset.png");
level->SetTexture(tileset);

// Position in world
level->SetOffset({0, 0});
level->SetScale(1.0f);

// Get spawn point
Vector2 spawn = level->GetPlayerSpawn();
player.SetPosition(spawn);
```

#### Rendering

```cpp
void RenderLevel()
{
    level->Draw();
}
```

#### Collision Detection

```cpp
// Check if position collides with level
Vector2 playerPos = player.GetPosition();
bool hitWall = level->CheckCollision(playerPos);

// Use in physics
if (level->CheckCollision({player.x + moveX, player.y}))
{
    // Blocked horizontally
    moveX = 0;
}

if (level->CheckCollision({player.x, player.y - 0.1f}))
{
    // On ground
    grounded = true;
}
```

#### Dynamic Level Editing

```cpp
// Destroy tiles (destructible terrain)
void DestroyTile(Level *level, Vector2 worldPos)
{
    int x = (int)worldPos.x;
    int y = (int)worldPos.y;
    level->RemoveTile(x, y);
}

// Build tiles (player construction)
void PlaceTile(Level *level, Vector2 worldPos)
{
    int x = (int)worldPos.x;
    int y = (int)worldPos.y;
    level->SetTile(x, y);
}

// Check tile state
bool IsSolid(Level *level, int x, int y)
{
    return level->GetTile(x, y);
}
```

#### Markers and Special Locations

```cpp
// Add markers during runtime
level->AddMarker("treasure", {15.0f, 20.0f});
level->AddMarker("enemy_spawn", {30.0f, 10.0f});

// Find and use markers
LevelMarker *goal = level->GetMarker("goal");
if (goal != nullptr)
{
    float distance = Vector2Distance(player.position, goal->position);
    if (distance < 2.0f)
    {
        // Player reached goal
        LevelComplete();
    }
}

// Iterate all markers
for (auto &marker : level->GetAllMarkers())
{
    if (marker.name.find("enemy") != std::string::npos)
    {
        SpawnEnemy(marker.position);
    }
}
```

#### Saving Levels

```cpp
// Save modified level
level->SaveToFile("levels/level1_modified.txt");

// Useful for level editor or procedural generation
```

## LevelManager Class

The LevelManager handles multiple levels and transitions.

### Basic Usage

```cpp
// Create manager
LevelManager levelManager;

// Load and add levels
Level *level1 = new Level();
level1->LoadFromFile("levels/level1.txt");
levelManager.AddLevel(level1);

Level *level2 = new Level();
level2->LoadFromFile("levels/level2.txt");
levelManager.AddLevel(level2);

Level *level3 = new Level();
level3->LoadFromFile("levels/level3.txt");
levelManager.AddLevel(level3);

// Start with first level
Level *current = levelManager.GetCurrentLevel();
```

### Level Navigation

```cpp
// Get current level
Level *level = levelManager.GetCurrentLevel();

// Switch to next level
if (playerReachedGoal)
{
    if (levelManager.NextLevel())
    {
        Level *newLevel = levelManager.GetCurrentLevel();
        player.SetPosition(newLevel->GetPlayerSpawn());
    }
    else
    {
        // No more levels - game complete!
        GameComplete();
    }
}

// Switch to previous level
if (playerFellOffMap)
{
    levelManager.PreviousLevel();
}

// Load specific level by index
levelManager.LoadLevel(2);  // Load level 3 (0-indexed)

// Load specific level by name
levelManager.LoadLevel("Boss Level");
```

### Level Queries

```cpp
// Get level count
int totalLevels = levelManager.GetLevelCount();

// Get current index
int currentIndex = levelManager.GetCurrentLevelIndex();

// Display progress
DrawText(TextFormat("Level %d / %d", currentIndex + 1, totalLevels), 
         10, 10, 20, WHITE);

// Get specific level without loading
Level *boss = levelManager.GetLevel("Boss Level");
if (boss != nullptr)
{
    TraceLog(LOG_INFO, "Boss level size: %dx%d", 
             boss->GetWidth(), boss->GetHeight());
}
```

## Complete Example: Game with Levels

```cpp
class Game
{
private:
    LevelManager levelManager;
    Player player;
    Texture2D tileset;
    bool levelComplete;
    
public:
    void Init()
    {
        // Load tileset
        tileset = GenerateGridTemplate();
        
        // Create levels
        for (int i = 1; i <= 3; ++i)
        {
            Level *level = new Level();
            std::string filename = TextFormat("builds/level%d.txt", i);
            
            if (level->LoadFromFile(filename))
            {
                level->SetTexture(tileset);
                level->SetOffset({0, 0});
                level->SetScale(1.0f);
                levelManager.AddLevel(level);
            }
            else
            {
                delete level;
            }
        }
        
        // Start first level
        StartLevel(levelManager.GetCurrentLevel());
    }
    
    void StartLevel(Level *level)
    {
        if (level != nullptr)
        {
            player.SetPosition(level->GetPlayerSpawn());
            levelComplete = false;
            TraceLog(LOG_INFO, "Started level: %s", level->GetName().c_str());
        }
    }
    
    void Update()
    {
        Level *level = levelManager.GetCurrentLevel();
        if (level == nullptr) return;
        
        // Player movement
        player.Update();
        
        // Collision with level
        Vector2 pos = player.GetPosition();
        if (level->CheckCollision(pos))
        {
            // Handle collision
            player.OnCollision();
        }
        
        // Check goal
        LevelMarker *goal = level->GetMarker("goal");
        if (goal != nullptr)
        {
            float dist = Vector2Distance(pos, goal->position);
            if (dist < 2.0f && !levelComplete)
            {
                levelComplete = true;
                LevelCompleted();
            }
        }
    }
    
    void LevelCompleted()
    {
        TraceLog(LOG_INFO, "Level completed!");
        
        // Move to next level after delay
        if (!levelManager.NextLevel())
        {
            // All levels complete
            TraceLog(LOG_INFO, "Game completed!");
        }
        else
        {
            StartLevel(levelManager.GetCurrentLevel());
        }
    }
    
    void Draw()
    {
        Level *level = levelManager.GetCurrentLevel();
        if (level != nullptr)
        {
            level->Draw();
        }
        player.Draw();
        
        // UI
        int current = levelManager.GetCurrentLevelIndex() + 1;
        int total = levelManager.GetLevelCount();
        DrawText(TextFormat("Level %d/%d", current, total), 10, 10, 20, WHITE);
    }
    
    ~Game()
    {
        UnloadTexture(tileset);
    }
};
```

## Advanced Features

### Custom Level Loaders

Extend the Level class to support different formats:

```cpp
class CustomLevel : public Level
{
public:
    bool LoadFromJSON(const std::string &filepath)
    {
        // Parse JSON level format
        // Set tiles, spawn, markers, etc.
        return true;
    }
    
    bool LoadFromBinary(const std::string &filepath)
    {
        // Load binary level format (faster, smaller)
        return true;
    }
};
```

### Procedural Levels

```cpp
Level* GenerateRandomLevel(int width, int height)
{
    Level *level = new Level(width, height, "Procedural");
    
    // Create floor
    for (int x = 0; x < width; ++x)
    {
        level->SetTile(x, 0);
        level->SetTile(x, height - 1);
    }
    
    // Create walls
    for (int y = 0; y < height; ++y)
    {
        level->SetTile(0, y);
        level->SetTile(width - 1, y);
    }
    
    // Random platforms
    for (int i = 0; i < 10; ++i)
    {
        int x = GetRandomValue(5, width - 10);
        int y = GetRandomValue(5, height - 5);
        int len = GetRandomValue(3, 8);
        
        for (int j = 0; j < len; ++j)
        {
            level->SetTile(x + j, y);
        }
    }
    
    level->SetPlayerSpawn({5.0f, (float)height - 5.0f});
    level->AddMarker("goal", {(float)width - 5.0f, (float)height - 5.0f});
    
    return level;
}
```

### Level Streaming

For large games, load/unload levels as needed:

```cpp
class StreamingLevelManager : public LevelManager
{
private:
    std::string levelFolder;
    
public:
    void PreloadLevel(int index)
    {
        // Load level in background thread
        std::string filename = TextFormat("%s/level%d.txt", 
                                         levelFolder.c_str(), index);
        Level *level = new Level();
        level->LoadFromFile(filename);
        AddLevel(level);
    }
    
    void UnloadLevel(int index)
    {
        // Free memory for distant levels
        Level *level = GetLevel(index);
        if (level != nullptr)
        {
            delete level;
            // Remove from vector
        }
    }
};
```

### Level Editor Mode

```cpp
class LevelEditor
{
private:
    Level *editingLevel;
    int selectedTileX, selectedTileY;
    
public:
    void Update()
    {
        Vector2 mouse = GetMousePosition();
        selectedTileX = (int)(mouse.x / TILE_SIZE);
        selectedTileY = (int)(mouse.y / TILE_SIZE);
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            editingLevel->SetTile(selectedTileX, selectedTileY);
        }
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        {
            editingLevel->RemoveTile(selectedTileX, selectedTileY);
        }
        
        if (IsKeyPressed(KEY_S))
        {
            editingLevel->SaveToFile("custom_level.txt");
        }
    }
    
    void Draw()
    {
        editingLevel->Draw();
        
        // Draw cursor
        DrawRectangleLines(selectedTileX * TILE_SIZE, 
                          selectedTileY * TILE_SIZE,
                          TILE_SIZE, TILE_SIZE, RED);
    }
};
```

## Performance Considerations

### Memory Usage

```cpp
// For a 128x64 level:
// Grid tiles: 128 × 64 × 1 byte = 8 KB
// Grid values: 129 × 65 × 1 byte = 8.4 KB
// Total per level: ~16-20 KB

// For 10 levels: ~200 KB total
// Very memory efficient!
```

### Loading Times

- Text file parsing is fast (< 10ms for typical levels)
- Consider binary format for 100+ levels
- Preload next level while playing current level

### Best Practices

1. **Reuse Textures**: Use the same tileset for all levels
2. **Level Size**: Keep under 256×256 for best performance
3. **Markers**: Use markers instead of hardcoding positions
4. **Save Often**: Auto-save player progress after each level
5. **Validation**: Check `IsLoaded()` before using a level

## Common Patterns

### Checkpoint System

```cpp
Vector2 currentCheckpoint;

void CheckCheckpoints(Level *level, Player *player)
{
    for (auto &marker : level->GetAllMarkers())
    {
        if (marker.name.find("checkpoint") != std::string::npos)
        {
            float dist = Vector2Distance(player->position, marker.position);
            if (dist < 2.0f)
            {
                currentCheckpoint = marker.position;
                TraceLog(LOG_INFO, "Checkpoint reached!");
            }
        }
    }
}

void PlayerDied()
{
    player.SetPosition(currentCheckpoint);
}
```

### Secret Areas

```cpp
void CheckSecrets(Level *level, Player *player)
{
    LevelMarker *secret = level->GetMarker("secret");
    if (secret != nullptr && !secretFound)
    {
        float dist = Vector2Distance(player->position, secret->position);
        if (dist < 2.0f)
        {
            secretFound = true;
            PlaySound(secretSound);
            player->coins += 100;
        }
    }
}
```

### Level Transitions

```cpp
void TransitionToLevel(int index)
{
    // Fade out
    for (float alpha = 0; alpha <= 1.0f; alpha += 0.02f)
    {
        BeginDrawing();
        currentLevel->Draw();
        DrawRectangle(0, 0, screenWidth, screenHeight, 
                     Fade(BLACK, alpha));
        EndDrawing();
    }
    
    // Switch level
    levelManager.LoadLevel(index);
    Level *newLevel = levelManager.GetCurrentLevel();
    player.SetPosition(newLevel->GetPlayerSpawn());
    
    // Fade in
    for (float alpha = 1.0f; alpha >= 0; alpha -= 0.02f)
    {
        BeginDrawing();
        newLevel->Draw();
        DrawRectangle(0, 0, screenWidth, screenHeight, 
                     Fade(BLACK, alpha));
        EndDrawing();
    }
}
```

## Troubleshooting

**Issue**: "Level file not found"
- Check file path is relative to executable location
- Use absolute paths for testing: `"C:/game/levels/level1.txt"`
- Verify file extension (.txt)

**Issue**: "Level loads but nothing renders"
- Ensure `SetTexture()` was called with valid texture
- Check `Draw()` is called in render loop
- Verify level has solid tiles (`GetTile()` returns true somewhere)

**Issue**: "Spawn position is inside wall"
- Check `@spawn` coordinates in level file
- Remember Y coordinates: bottom = 0, top = height
- Test with: `DrawCircle(spawn.x * 32, spawn.y * 32, 5, RED);`

**Issue**: "Collision not working"
- Ensure grid coordinates match world coordinates
- Check PIXELS_PER_UNIT matches your coordinate system
- Verify tiles are actually solid with `level->GetTile(x, y)`

## Example Level Files

Three example levels are included in `builds/`:
- **level1.txt**: Tutorial level with basic platforming
- **level2.txt**: Intermediate level with checkpoints
- **level3.txt**: Advanced level with boss arena and secrets

## Future Enhancements

- [ ] Background layers (parallax scrolling)
- [ ] Animated tiles (water, lava, etc.)
- [ ] Level streaming for seamless transitions
- [ ] Tile properties (slippery, damage, etc.)
- [ ] Enemy spawn points with types
- [ ] Level scripting/events
- [ ] Compressed level format
- [ ] Tileset switching per level
