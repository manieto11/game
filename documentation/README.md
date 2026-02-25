# Game Engine Documentation

This documentation covers the core systems and components of the 2D platformer game engine.

## Overview

This is a custom 2D game engine built with C++ and Raylib, designed for platformer games. The engine uses a component-based architecture with the following core systems:

- **Transform System**: Handles position, scale, and rotation of game objects
- **Collision System**: Provides collision detection with box and grid-based colliders
- **Physics System**: Implements gravity and velocity-based movement
- **Rendering System**: Efficient sprite rendering with texture management
- **Grid System**: Dual-grid tile system for level design

## Quick Start

1. **Building the Project**
   - Windows: `mingw32-make -f Makefile.win`
   - Linux/Mac: `make`

2. **Basic Game Object Setup**
```cpp
// Create a game object with transform
Transform2D transform(100.0f, 100.0f);

// Add sprite renderer
SpriteRenderer renderer(&transform);
renderer.SetTexture(&myTexture);

// Add physics
BoxCollider collider(&transform, {0, 0, 32, 32});
RigidBody rigidbody(&transform, &collider);

// In game loop
rigidbody.ApplyPhysics();
renderer.Draw();
```

## Documentation Files

- [Collision System](collision_system.md) - Collision detection and physics
- [Rendering System](rendering_system.md) - Sprite rendering and optimization
- [Component System](component_system.md) - Transform and component architecture
- [Grid System](grid_system.md) - Tile-based level design
- [Level System](level_system.md) - Creating, loading, and managing game levels

## Architecture

The engine follows an entity-component pattern where game objects are composed of:
- A **Transform2D** component (position, scale, rotation)
- Optional components like SpriteRenderer, Collider, RigidBody
- Components reference the transform for positioning

## Configuration

Game settings are defined in `include/settings.h`:
- `PIXELS_PER_UNIT`: Pixels per world unit (default: 32)
- `FIXED_DELTA_TIME`: Fixed timestep for physics (default: 0.02s)
- `DEFAULT_GRAVITY`: Default gravity value (default: 8.0)

## Dependencies

- **Raylib**: Graphics, input, and windowing
- **C++**: Modern C++ (C++11 or higher)
- **MinGW/GCC**: Compiler toolchain
