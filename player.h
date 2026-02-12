#ifndef PLAYER_H
#define PLAYER_H

#include "components.h"
#include "gameObject.h"

class Player : public GameObject
{
public:
    Transform2D transform;
    SpriteRenderer spriteRenderer;
    Collider collider;
    RigidBody rigidBody;
    Player();
    void Initialize();
    void Update();
    void FixedUpdate();
    void Finalize();
};

#endif