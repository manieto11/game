#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

extern Entity *PlayerEntity;

void InitPlayer();
void DrawPlayer();
void DrawPlayerDebug();
void UpdatePlayer();

#endif