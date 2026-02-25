# Rendering System Documentation

## Overview

The rendering system handles sprite drawing with support for textures, scaling, rotation, and color tinting. It's designed for efficiency with texture caching and optimized draw calls.

## SpriteRenderer Component

The `SpriteRenderer` class is responsible for rendering sprites for game objects.

### Class Structure

```cpp
class SpriteRenderer
{
private:
    Transform2D *transform;  // Position/scale/rotation
    bool textureValid;       // Cached texture validity

public:
    Texture2D *texture;      // Texture to render
    Rectangle spriteRec;     // Source rectangle in texture
    Color color;             // Tint color

    SpriteRenderer(Transform2D *transform);
    void SetTexture(Texture2D *newTexture);
    void Draw();
};
```

### Basic Usage

```cpp
// Create sprite renderer
Transform2D transform(100.0f, 50.0f);
SpriteRenderer renderer(&transform);

// Set custom texture
Texture2D playerTexture = LoadTexture("player.png");
renderer.SetTexture(&playerTexture);

// Set sprite region (for sprite sheets)
renderer.spriteRec = {0, 0, 32, 32};

// Set tint color
renderer.color = WHITE;

// In render loop
renderer.Draw();
```

## Optimization Features

### 1. Texture Validation Caching

The renderer caches texture validity to avoid checking every frame:

```cpp
// Validation happens only when texture changes
renderer.SetTexture(&newTexture); // Validates here
renderer.Draw();                  // No validation, just draws
```

### 2. Early Return on Invalid Texture

Invalid textures are skipped immediately without additional processing:

```cpp
void Draw()
{
    if (!textureValid)
        return; // Early exit
    
    // ... rendering code
}
```

### 3. Variable Caching

Commonly used calculations are cached to reduce redundant operations:

```cpp
// Cache scale calculations
const float scaleX = transform->scale.x * PIXELS_PER_UNIT;
const float scaleY = transform->scale.y * PIXELS_PER_UNIT;

// Use cached values multiple times
Rectangle dest = {x, y, scaleX, scaleY};
Vector2 origin = {scaleX, -scaleY / 2};
```

## Coordinate System

The engine uses a custom coordinate system that differs from screen space:

### World Space to Screen Space Conversion

```
Screen X = (World X + Scale Y / 2) * PIXELS_PER_UNIT
Screen Y = -(World Y + Scale Y) * PIXELS_PER_UNIT
```

- **Y-axis is inverted**: Positive Y is up in world space, down in screen space
- **Origin offset**: Applied to center sprites correctly
- **PIXELS_PER_UNIT**: Converts world units to screen pixels (default: 32)

### Example

```cpp
// World position
transform.position = {10.0f, 5.0f};  // 10 units right, 5 units up
transform.scale = {1.0f, 2.0f};       // 1 unit wide, 2 units tall

// Screen position (with PIXELS_PER_UNIT = 32)
screenX = (10.0 + 2.0/2) * 32 = 352 pixels
screenY = -(5.0 + 2.0) * 32 = -224 pixels
```

## Sprite Sheets and Animation

While animation isn't built-in, you can implement it by changing `spriteRec`:

```cpp
class AnimatedSprite
{
private:
    SpriteRenderer *renderer;
    int currentFrame;
    float frameTime;
    
public:
    void Update(float deltaTime)
    {
        frameTime += deltaTime;
        if (frameTime >= 0.1f) // 10 FPS animation
        {
            currentFrame = (currentFrame + 1) % 4; // 4 frames
            renderer->spriteRec.x = currentFrame * 32;
            frameTime = 0;
        }
    }
};
```

## Rotation and Origin

The renderer supports rotation around a configurable origin point:

```cpp
// Origin is calculated for each draw call
Vector2 origin = {scaleX, -scaleY / 2};

// This centers rotation horizontally and vertically
DrawTexturePro(*texture, spriteRec, dest, origin, rotation, color);
```

To rotate around a different point, you would need to modify the origin calculation.

## Color Tinting

The `color` property applies a tint to the sprite:

```cpp
// Normal rendering
renderer.color = WHITE;

// Red tint (damage effect)
renderer.color = RED;

// Semi-transparent
renderer.color = {255, 255, 255, 128};

// Custom color
renderer.color = {100, 200, 150, 255};
```

## Performance Tips

### 1. Texture Management

```cpp
// GOOD: Load textures once, reuse
Texture2D globalPlayerTexture;
void Init() { globalPlayerTexture = LoadTexture("player.png"); }
void CreatePlayer() { renderer.SetTexture(&globalPlayerTexture); }

// BAD: Loading texture for each sprite
void CreatePlayer() {
    Texture2D tex = LoadTexture("player.png"); // Memory leak!
    renderer.SetTexture(&tex);
}
```

### 2. Minimize Texture Changes

Group draw calls by texture to reduce state changes:

```cpp
// GOOD: Draw all enemies, then all pickups
for (auto& enemy : enemies) enemy.renderer.Draw();
for (auto& pickup : pickups) pickup.renderer.Draw();

// LESS OPTIMAL: Alternating textures
for (auto& obj : allObjects) obj.renderer.Draw();
```

### 3. Cull Off-Screen Objects

```cpp
bool IsOnScreen(Transform2D *t)
{
    float screenX = t->position.x * PIXELS_PER_UNIT;
    float screenY = -t->position.y * PIXELS_PER_UNIT;
    return (screenX > -64 && screenX < screenWidth + 64 &&
            screenY > -64 && screenY < screenHeight + 64);
}

void DrawAll()
{
    for (auto& obj : objects)
    {
        if (IsOnScreen(&obj.transform))
            obj.renderer.Draw();
    }
}
```

## Grid System Rendering

The Grid class has its own rendering system for tile-based levels:

```cpp
Grid levelGrid(64, 64);
levelGrid.texture = LoadTexture("tileset.png");
levelGrid.DrawTiles(); // Renders all visible tiles
```

Grid rendering uses a marching squares algorithm for efficient tile rendering. See [grid_system.md](grid_system.md) for details.

## Common Issues

**Issue**: "Sprite not appearing"
- Check texture is loaded: `IsTextureValid(texture)`
- Verify position is on-screen
- Ensure Draw() is called in render loop
- Check color alpha isn't 0

**Issue**: "Sprite appears in wrong position"
- Remember Y-axis is inverted
- Check PIXELS_PER_UNIT matches your coordinate system
- Verify transform position is in world space

**Issue**: "Performance drops with many sprites"
- Implement frustum culling (off-screen culling)
- Batch sprites by texture
- Use texture atlases for sprite sheets
- Consider spatial partitioning for large levels

## Future Enhancements

- [ ] Built-in animation system
- [ ] Sprite layering/sorting
- [ ] Camera system with viewport culling
- [ ] Batch rendering for identical sprites
- [ ] Shader support for effects
- [ ] Particle system integration
