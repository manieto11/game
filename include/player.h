#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "platform.h"

extern Entity *PlayerEntity;

void InitPlayer();
void DrawPlayer();
void UpdatePlayer(Platform** platforms, int platformCount);

#endif