#ifndef PLAYER_H
#define PLAYER_H

#include "components.h"
#include "game_object.h"

class Player : public GameObject
{
public:
    Transform2D* transform;
    SpriteRenderer* spriteRenderer;
    Collider* collider;
    RigidBody* rigidBody;
    PlayerController* controller;

    Player();
    ~Player();

    void Initialize();
    void Update();
    void FixedUpdate();
    void Draw();
    void Finalize();
};

#endif