# Collision System Documentation

## Overview

The collision system provides collision detection capabilities through different collider types. It integrates with the physics system to enable ground detection, wall collision, and interactive objects.

## Components

### Base Collider Class

```cpp
class Collider
{
public:
    virtual bool Colliding(const Vector2 point) = 0;
};
```

The base class defines the interface for all colliders. Any collider must implement the `Colliding` method to check if a point is within its bounds.

### BoxCollider

A rectangular collider that can be attached to a transform or used independently.

#### Constructors

```cpp
BoxCollider()                                    // Empty collider
BoxCollider(Transform2D *transform)              // Unit-sized box
BoxCollider(Transform2D *transform, Rectangle rect) // Custom rectangle
```

#### Usage

```cpp
// Create a box collider
Transform2D transform(10.0f, 5.0f);
Rectangle bounds = {0, 0, 1.0f, 2.0f}; // Local space
BoxCollider collider(&transform, bounds);

// Check collision
Vector2 point = {10.5f, 6.0f};
bool isColliding = collider.Colliding(point);
```

#### Implementation Details

The BoxCollider works in two modes:

1. **World Space Mode** (transform = nullptr):
   - The rectangle is treated as world coordinates
   - Direct boundary checking: `point.x >= rect.x && point.x <= rect.x + rect.width`

2. **Transform Mode** (transform set):
   - Rectangle is in local space relative to transform
   - Bounds are calculated as:
     ```
     left = transform->position.x + rectangle.x
     right = left + rectangle.width * transform->scale.x
     bottom = transform->position.y + rectangle.y
     top = bottom + rectangle.height * transform->scale.y
     ```
   - Scale is applied to width/height

**Note**: Rotation is not currently supported in collision detection. This is a planned feature for future versions.

### GridCollider

A tile-based collider that uses the Grid system for level geometry.

#### Constructors

```cpp
GridCollider()                          // Empty collider
GridCollider(Grid *grid)                // Attach to grid
GridCollider(Grid *grid, Vector2 origin) // With origin offset
```

#### Usage

```cpp
// Create a grid-based level
Grid levelGrid(64, 64);
levelGrid.LoadTilesFromString(mapData, dataSize, '#');

// Create grid collider
GridCollider collider(&levelGrid);

// Check collision at world position
Vector2 playerPos = {15.5f, 20.3f};
bool hitGround = collider.Colliding(playerPos);
```

#### Implementation Details

- Converts world coordinates to grid coordinates: `gridX = (int)point.x`
- Returns `true` if the tile at that position is solid
- Returns `false` if grid is null or position is out of bounds
- Each grid tile is 1 unit in world space

## Integration with Physics

The collision system is used by the `RigidBody` class to detect ground collisions and prevent objects from falling through solid geometry.

### Example: Ground Detection

```cpp
void CheckGrounded(RigidBody *rb, Collider *groundCollider)
{
    // Check multiple points along the bottom of the object
    Vector2 bottomLeft = {transform.position.x, transform.position.y - 0.1f};
    Vector2 bottomRight = {transform.position.x + 1.0f, transform.position.y - 0.1f};
    
    bool grounded = groundCollider->Colliding(bottomLeft) || 
                    groundCollider->Colliding(bottomRight);
    
    if (grounded && rb->velocity.y < 0)
    {
        rb->velocity.y = 0;
        // Stop falling
    }
}
```

## Best Practices

1. **Collider Sizing**: Make colliders slightly smaller than sprites to avoid visual clipping
   ```cpp
   // For a 32x32 sprite, use a 28x28 collider
   Rectangle bounds = {2, 2, 28, 28};
   ```

2. **Multiple Point Testing**: For larger objects, test multiple points along edges
   ```cpp
   bool CheckSideCollision(Collider *col, Transform2D *t)
   {
       for (float y = 0; y < height; y += 0.5f)
       {
           if (col->Colliding({t->position.x, t->position.y + y}))
               return true;
       }
       return false;
   }
   ```

3. **Grid Alignment**: Ensure grid tile size matches your world unit scale
   - If PIXELS_PER_UNIT = 32 and TILE_SIZE = 32, tiles are 1x1 world units

4. **Performance**: 
   - GridCollider is fast for static level geometry
   - BoxCollider is better for moving objects
   - Cache collision results when possible

## Planned Features

- [ ] Rotation support for BoxCollider
- [ ] Circle/Capsule colliders
- [ ] Box-to-box collision detection
- [ ] Collision layers and filtering
- [ ] Trigger volumes (non-solid collision zones)
- [ ] Collision callbacks/events

## Common Issues

**Issue**: "Collider not detecting collision"
- Ensure transform is properly set
- Check that world coordinates are correct
- Verify grid tiles are loaded with `grid->GetTile(x, y)`

**Issue**: "Player falls through floor"
- Check collision detection runs before position updates
- Ensure collision points are at the bottom of the player
- Verify grid collider has solid tiles at floor positions
