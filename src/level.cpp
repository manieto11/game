#include "level.h"
#include <fstream>
#include <sstream>

// ========== Level Implementation ==========

Level::Level() 
    : grid(nullptr), collider(nullptr), levelName("Untitled"), 
      levelWidth(0), levelHeight(0), playerSpawn({0, 0}), loaded(false)
{
}

Level::Level(int width, int height, const std::string &name)
    : levelName(name), levelWidth(width), levelHeight(height), 
      playerSpawn({0, 0}), loaded(false)
{
    grid = new Grid(width, height);
    collider = new GridCollider(grid);
    loaded = true;
}

Level::~Level()
{
    if (collider != nullptr)
        delete collider;
    if (grid != nullptr)
        delete grid;
}

bool Level::LoadFromFile(const std::string &filepath, char solidChar)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        TraceLog(LOG_ERROR, "[Level] Failed to open file: %s", filepath.c_str());
        return false;
    }

    std::string line;
    std::vector<std::string> lines;
    int maxWidth = 0;

    // Read all lines and find dimensions
    while (std::getline(file, line))
    {
        // Skip metadata lines (starting with @)
        if (line.length() > 0 && line[0] == '@')
        {
            // Parse metadata
            if (line.find("@spawn") == 0)
            {
                std::istringstream iss(line);
                std::string cmd;
                float x, y;
                if (iss >> cmd >> x >> y)
                {
                    playerSpawn = {x, y};
                }
            }
            else if (line.find("@marker") == 0)
            {
                std::istringstream iss(line);
                std::string cmd, name;
                float x, y;
                if (iss >> cmd >> name >> x >> y)
                {
                    AddMarker(name, {x, y});
                }
            }
            continue;
        }

        lines.push_back(line);
        if (line.length() > maxWidth)
            maxWidth = line.length();
    }

    file.close();

    if (lines.empty())
    {
        TraceLog(LOG_ERROR, "[Level] File is empty: %s", filepath.c_str());
        return false;
    }

    levelHeight = lines.size();
    levelWidth = maxWidth;

    // Initialize grid if needed
    if (grid == nullptr)
    {
        grid = new Grid(levelWidth, levelHeight);
        collider = new GridCollider(grid);
    }

    // Convert lines to grid data (flip Y-axis: top line = highest Y)
    std::string gridData;
    for (int y = levelHeight - 1; y >= 0; --y)
    {
        for (int x = 0; x < levelWidth; ++x)
        {
            if (x < lines[y].length())
                gridData += lines[y][x];
            else
                gridData += ' ';  // Pad with spaces
        }
    }

    grid->LoadTilesFromString(gridData.c_str(), gridData.length(), solidChar);
    loaded = true;

    TraceLog(LOG_INFO, "[Level] Loaded level: %s (%dx%d)", 
             levelName.c_str(), levelWidth, levelHeight);
    return true;
}

bool Level::LoadFromString(const std::string &data, int width, int height, char solidChar)
{
    levelWidth = width;
    levelHeight = height;

    if (grid == nullptr)
    {
        grid = new Grid(width, height);
        collider = new GridCollider(grid);
    }

    grid->LoadTilesFromString(data.c_str(), data.length(), solidChar);
    loaded = true;

    TraceLog(LOG_INFO, "[Level] Loaded level from string: %s (%dx%d)", 
             levelName.c_str(), width, height);
    return true;
}

bool Level::SaveToFile(const std::string &filepath) const
{
    if (!loaded || grid == nullptr)
    {
        TraceLog(LOG_ERROR, "[Level] Cannot save unloaded level");
        return false;
    }

    std::ofstream file(filepath);
    if (!file.is_open())
    {
        TraceLog(LOG_ERROR, "[Level] Failed to create file: %s", filepath.c_str());
        return false;
    }

    // Write metadata
    file << "@spawn " << playerSpawn.x << " " << playerSpawn.y << "\n";
    
    for (const auto &marker : markers)
    {
        file << "@marker " << marker.name << " " 
             << marker.position.x << " " << marker.position.y << "\n";
    }

    // Write grid data (flip Y-axis: highest Y = top line)
    for (int y = levelHeight - 1; y >= 0; --y)
    {
        for (int x = 0; x < levelWidth; ++x)
        {
            file << (grid->GetTile(x, y) ? '#' : '.');
        }
        file << "\n";
    }

    file.close();
    TraceLog(LOG_INFO, "[Level] Saved level to: %s", filepath.c_str());
    return true;
}

void Level::Draw() const
{
    if (loaded && grid != nullptr)
    {
        grid->DrawTiles();
    }
}

bool Level::CheckCollision(Vector2 position) const
{
    if (!loaded || collider == nullptr)
        return false;
    
    return collider->Colliding(position);
}

void Level::SetTile(int x, int y)
{
    if (grid != nullptr)
        grid->SetTile(x, y);
}

void Level::RemoveTile(int x, int y)
{
    if (grid != nullptr)
        grid->RemoveTile(x, y);
}

bool Level::GetTile(int x, int y) const
{
    if (grid != nullptr)
        return grid->GetTile(x, y);
    return false;
}

void Level::SetPlayerSpawn(Vector2 position)
{
    playerSpawn = position;
}

Vector2 Level::GetPlayerSpawn() const
{
    return playerSpawn;
}

void Level::AddMarker(const std::string &name, Vector2 position)
{
    markers.push_back({position, name});
}

LevelMarker* Level::GetMarker(const std::string &name)
{
    for (auto &marker : markers)
    {
        if (marker.name == name)
            return &marker;
    }
    return nullptr;
}

std::vector<LevelMarker>& Level::GetAllMarkers()
{
    return markers;
}

void Level::SetTexture(Texture2D texture)
{
    if (grid != nullptr)
        grid->texture = texture;
}

void Level::SetOffset(Vector2 offset)
{
    if (grid != nullptr)
        grid->SetOffset(offset);
}

void Level::SetScale(float scale)
{
    if (grid != nullptr)
        grid->SetScale(scale);
}

std::string Level::GetName() const
{
    return levelName;
}

bool Level::IsLoaded() const
{
    return loaded;
}

int Level::GetWidth() const
{
    return levelWidth;
}

int Level::GetHeight() const
{
    return levelHeight;
}

Grid* Level::GetGrid() const
{
    return grid;
}

GridCollider *Level::GetGridCollider() const
{
    return collider;
}

// ========== LevelManager Implementation ==========

LevelManager::LevelManager() : currentLevel(nullptr), currentLevelIndex(-1)
{
}

LevelManager::~LevelManager()
{
    Clear();
}

void LevelManager::AddLevel(Level *level)
{
    if (level != nullptr)
    {
        levels.push_back(level);
        
        // Set first level as current if none is set
        if (currentLevel == nullptr)
        {
            currentLevel = level;
            currentLevelIndex = 0;
        }
    }
}

bool LevelManager::LoadLevel(int index)
{
    if (index < 0 || index >= levels.size())
    {
        TraceLog(LOG_ERROR, "[LevelManager] Invalid level index: %d", index);
        return false;
    }

    currentLevel = levels[index];
    currentLevelIndex = index;
    
    TraceLog(LOG_INFO, "[LevelManager] Loaded level %d: %s", 
             index, currentLevel->GetName().c_str());
    return true;
}

bool LevelManager::LoadLevel(const std::string &name)
{
    for (int i = 0; i < levels.size(); ++i)
    {
        if (levels[i]->GetName() == name)
        {
            currentLevel = levels[i];
            currentLevelIndex = i;
            TraceLog(LOG_INFO, "[LevelManager] Loaded level: %s", name.c_str());
            return true;
        }
    }
    
    TraceLog(LOG_ERROR, "[LevelManager] Level not found: %s", name.c_str());
    return false;
}

Level* LevelManager::GetCurrentLevel() const
{
    return currentLevel;
}

Level* LevelManager::GetLevel(int index) const
{
    if (index >= 0 && index < levels.size())
        return levels[index];
    return nullptr;
}

Level* LevelManager::GetLevel(const std::string &name) const
{
    for (auto level : levels)
    {
        if (level->GetName() == name)
            return level;
    }
    return nullptr;
}

bool LevelManager::NextLevel()
{
    if (currentLevelIndex + 1 < levels.size())
    {
        return LoadLevel(currentLevelIndex + 1);
    }
    return false;
}

bool LevelManager::PreviousLevel()
{
    if (currentLevelIndex > 0)
    {
        return LoadLevel(currentLevelIndex - 1);
    }
    return false;
}

int LevelManager::GetLevelCount() const
{
    return levels.size();
}

int LevelManager::GetCurrentLevelIndex() const
{
    return currentLevelIndex;
}

void LevelManager::Clear()
{
    for (auto level : levels)
    {
        delete level;
    }
    levels.clear();
    currentLevel = nullptr;
    currentLevelIndex = -1;
}
