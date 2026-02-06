#ifndef PLAYER_H
#define PLAYER_H

#include "gameObject.h"

class Player : public virtual GameObject
{
private:
    Transform2D transform;
    SpriteRenderer spriteRenderer;
public:
    Player();
    void Initialize();
    void Update();
    void FixedUpdate();
    void Finalize();
};

#endif