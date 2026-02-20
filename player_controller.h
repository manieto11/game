#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "rigid_body.h"

class PlayerController
{
    RigidBody *rigidBody;
    bool enabled;

public:
    float moveSpeed;
    PlayerController();
    PlayerController(RigidBody *playerRigidBody);
    void SetEnabled(bool value);
    void Update();
};

#endif