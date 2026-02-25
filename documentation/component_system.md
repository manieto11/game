# Component System Documentation

## Overview

The engine uses a component-based architecture where game objects are composed of independent, reusable components. Each component focuses on a specific aspect of object behavior.

## Core Components

### Transform2D

The fundamental component that stores position, scale, and rotation.

#### Class Definition

```cpp
class Transform2D
{
public:
    Vector2 position;  // World position (x, y)
    Vector2 scale;     // Scale factors (x, y)
    int rotation;      // Rotation in degrees

    Transform2D();
    Transform2D(int x, int y);
    Transform2D(float x, float y);

    void Move(int x, int y);
    void Move(float x, float y);
    void Resize(float sizeMultiplier);
    void Resize(float sizeMultiplierX, float sizeMultiplierY);
    void Rotate(int degrees);
};
```

#### Usage

```cpp
// Create transform
Transform2D transform(100.0f, 50.0f);  // Position at (100, 50)
transform.scale = {1.0f, 1.0f};        // Default unit scale
transform.rotation = 0;                 // No rotation

// Move object
transform.Move(5.0f, 0.0f);  // Move 5 units right
transform.position.x += 10;   // Alternative: direct access

// Scale object
transform.Resize(2.0f);           // Scale by 2x (uniform)
transform.Resize(1.5f, 0.5f);     // Scale 1.5x width, 0.5x height

// Rotate object
transform.Rotate(45);             // Rotate 45 degrees
transform.rotation += 90;         // Alternative: direct access
```

#### Coordinate System

- **Position**: World coordinates (center of object)
- **Scale**: Multipliers applied to base size
- **Rotation**: Degrees clockwise (0° = right, 90° = down, 180° = left, 270° = up)

### RigidBody

Handles physics simulation with velocity and gravity.

#### Class Definition

```cpp
class RigidBody
{
public:
    Vector2 velocity;  // Movement velocity (units/second)
    float gravity;     // Gravity strength

    RigidBody(Transform2D *transform, Collider *collider);
    RigidBody(Transform2D *transform, Collider *collider, float gravity);
    void ApplyPhysics();
};
```

#### Usage

```cpp
// Create rigidbody
BoxCollider collider(&transform);
RigidBody rb(&transform, &collider);

// Set velocity
rb.velocity = {5.0f, 0.0f};  // Move right at 5 units/sec

// Jump
rb.velocity.y = 10.0f;  // Upward velocity

// Custom gravity
RigidBody rb2(&transform, &collider, 15.0f);  // Stronger gravity

// Update in game loop
void FixedUpdate()
{
    rb.ApplyPhysics();  // Applies gravity and moves transform
}
```

#### Implementation Details

**ApplyPhysics()** performs:
1. Apply gravity to Y velocity (if not grounded)
2. Move transform by velocity * FIXED_DELTA_TIME

**Gravity Behavior**:
- Only applied when not grounded
- Negative Y velocity (falling) is stopped when grounded
- Default gravity: 8.0 units/second²

### Collider (BoxCollider / GridCollider)

Provides collision detection. See [collision_system.md](collision_system.md) for details.

### SpriteRenderer

Renders sprites. See [rendering_system.md](rendering_system.md) for details.

## Component Relationships

Components often reference each other to work together:

```
GameObject
├── Transform2D (position, scale, rotation)
│   ├── → SpriteRenderer (rendering)
│   ├── → BoxCollider (collision bounds)
│   └── → RigidBody (physics)
```

### Example: Player Object

```cpp
class Player
{
public:
    Transform2D transform;
    SpriteRenderer renderer;
    BoxCollider collider;
    RigidBody rigidbody;
    
    Player(float x, float y) 
        : transform(x, y),
          renderer(&transform),
          collider(&transform, {0, 0, 1, 1}),
          rigidbody(&transform, &collider)
    {
        transform.scale = {1.0f, 1.0f};
        renderer.SetTexture(&PlayerTexture);
    }
    
    void Update(float deltaTime)
    {
        // Handle input
        if (IsKeyDown(KEY_RIGHT))
            rigidbody.velocity.x = PLAYER_SPEED;
        else if (IsKeyDown(KEY_LEFT))
            rigidbody.velocity.x = -PLAYER_SPEED;
        else
            rigidbody.velocity.x = 0;
            
        if (IsKeyPressed(KEY_SPACE))
            rigidbody.velocity.y = 10.0f;  // Jump
    }
    
    void FixedUpdate()
    {
        rigidbody.ApplyPhysics();
    }
    
    void Draw()
    {
        renderer.Draw();
    }
};
```

## Component Initialization Order

**Important**: Initialize components in the correct order to avoid null pointer issues:

```cpp
// CORRECT ORDER
Transform2D transform(x, y);          // 1. Create transform first
SpriteRenderer renderer(&transform);   // 2. Components reference transform
BoxCollider collider(&transform);      // 3. More components
RigidBody rb(&transform, &collider);   // 4. RigidBody references both

// INCORRECT - Will crash!
BoxCollider collider(&transform);      // Transform doesn't exist yet!
Transform2D transform(x, y);
```

## Update Loops

Different systems update at different times:

### Game Loop Structure

```cpp
void GameLoop()
{
    float accumulator = 0.0f;
    
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        accumulator += deltaTime;
        
        // Input & Game Logic (every frame)
        player.Update(deltaTime);
        
        // Fixed Physics Update (fixed timestep)
        while (accumulator >= FIXED_DELTA_TIME)
        {
            player.FixedUpdate();
            accumulator -= FIXED_DELTA_TIME;
        }
        
        // Rendering (every frame)
        BeginDrawing();
        ClearBackground(BLACK);
        player.Draw();
        EndDrawing();
    }
}
```

### Update Functions

- **Update(deltaTime)**: Called every frame for input, logic, animations
- **FixedUpdate()**: Called at fixed intervals (FIXED_DELTA_TIME) for physics
- **Draw()**: Called every frame for rendering

## Creating Custom Components

You can extend the system with custom components:

```cpp
class HealthComponent
{
private:
    Transform2D *transform;
    int maxHealth;
    int currentHealth;
    
public:
    HealthComponent(Transform2D *t, int health) 
        : transform(t), maxHealth(health), currentHealth(health) {}
    
    void TakeDamage(int amount)
    {
        currentHealth -= amount;
        if (currentHealth <= 0)
            OnDeath();
    }
    
    void Heal(int amount)
    {
        currentHealth = (currentHealth + amount > maxHealth) 
                        ? maxHealth 
                        : currentHealth + amount;
    }
    
    float GetHealthPercent() const
    {
        return (float)currentHealth / maxHealth;
    }
    
private:
    void OnDeath()
    {
        // Handle death
        TraceLog(LOG_INFO, "Object died at position (%f, %f)", 
                 transform->position.x, transform->position.y);
    }
};
```

## Best Practices

### 1. Component Ownership

Use clear ownership patterns:

```cpp
// GOOD: Player owns all components
class Player
{
    Transform2D transform;
    SpriteRenderer renderer;
    // ... components initialized with &transform
};

// BAD: Unclear ownership
Transform2D *sharedTransform = new Transform2D();
SpriteRenderer *renderer1 = new SpriteRenderer(sharedTransform);
SpriteRenderer *renderer2 = new SpriteRenderer(sharedTransform);
// Who deletes sharedTransform?
```

### 2. Null Checking

Always validate pointers in component constructors:

```cpp
RigidBody::RigidBody(Transform2D *t, Collider *c)
{
    if (t == nullptr)
        TraceLog(LOG_ERROR, "RigidBody: Transform is null!");
    if (c == nullptr)
        TraceLog(LOG_ERROR, "RigidBody: Collider is null!");
    // ... continue with initialization
}
```

### 3. Component Communication

Use direct references or getter methods:

```cpp
class Enemy
{
public:
    Transform2D transform;
    HealthComponent health;
    
    // Expose methods for other systems
    Vector2 GetPosition() const { return transform.position; }
    bool IsAlive() const { return health.GetHealthPercent() > 0; }
};

// Another system can check
if (enemy.IsAlive() && Distance(player.GetPosition(), enemy.GetPosition()) < 5.0f)
{
    enemy.health.TakeDamage(10);
}
```

## Physics Constants

Defined in `settings.h`:

```cpp
#define FIXED_DELTA_TIME 0.02f     // 50 updates/second
#define DEFAULT_GRAVITY 8.0f        // Default gravity strength
#define PIXELS_PER_UNIT 32          // Screen pixels per world unit
#define PLAYER_SPEED 5.0f           // Player movement speed
```

## Future Enhancements

- [ ] Component Manager/Registry system
- [ ] Entity IDs and lookup
- [ ] Component serialization (save/load)
- [ ] Event system for component communication
- [ ] Component pooling for performance
- [ ] Debug visualization for components
