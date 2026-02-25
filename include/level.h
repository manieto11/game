#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"
#include "dual_grid_system.h"
#include "collider.h"
#include <string>
#include <vector>

// Represents a spawn point or special location in the level
struct LevelMarker
{
    Vector2 position;
    std::string name;
};

// A complete game level with tiles, collision, and metadata
class Level
{
private:
    Grid *grid;
    GridCollider *collider;
    std::string levelName;
    int levelWidth, levelHeight;
    Vector2 playerSpawn;
    std::vector<LevelMarker> markers;
    bool loaded;

public:
    Level();
    Level(int width, int height, const std::string &name);
    ~Level();

    // Load level from a text file
    bool LoadFromFile(const std::string &filepath, char solidChar = '#');
    
    // Load level from string data
    bool LoadFromString(const std::string &data, int width, int height, char solidChar = '#');
    
    // Save level to a text file
    bool SaveToFile(const std::string &filepath) const;
    
    // Render the level
    void Draw() const;
    
    // Check collision at a point
    bool CheckCollision(Vector2 position) const;
    
    // Tile manipulation
    void SetTile(int x, int y);
    void RemoveTile(int x, int y);
    bool GetTile(int x, int y) const;
    
    // Level properties
    void SetPlayerSpawn(Vector2 position);
    Vector2 GetPlayerSpawn() const;
    
    void AddMarker(const std::string &name, Vector2 position);
    LevelMarker* GetMarker(const std::string &name);
    std::vector<LevelMarker>& GetAllMarkers();
    
    void SetTexture(Texture2D texture);
    void SetOffset(Vector2 offset);
    void SetScale(float scale);
    
    std::string GetName() const;
    bool IsLoaded() const;
    
    int GetWidth() const;
    int GetHeight() const;
    
    Grid* GetGrid() const;

    GridCollider* GetGridCollider() const;
};

// Manages multiple levels and handles transitions
class LevelManager
{
private:
    std::vector<Level*> levels;
    Level *currentLevel;
    int currentLevelIndex;

public:
    LevelManager();
    ~LevelManager();
    
    // Add a level to the manager
    void AddLevel(Level *level);
    
    // Load a level by index
    bool LoadLevel(int index);
    
    // Load a level by name
    bool LoadLevel(const std::string &name);
    
    // Get current level
    Level* GetCurrentLevel() const;
    
    // Get level by index
    Level* GetLevel(int index) const;
    
    // Get level by name
    Level* GetLevel(const std::string &name) const;
    
    // Level navigation
    bool NextLevel();
    bool PreviousLevel();
    
    int GetLevelCount() const;
    int GetCurrentLevelIndex() const;
    
    void Clear();
};

#endif
