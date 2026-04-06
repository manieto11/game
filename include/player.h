#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "entity.h"
#include "platform.h"

extern Entity *PlayerEntity;

void InitPlayer();
void DrawPlayer();
void DrawPlayerDebug();
void UpdatePlayer(std::vector<Platform*>);

#endif