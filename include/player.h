#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <vector>
#include "entity.h"
#include "platform.h"

extern Entity *PlayerEntity;

void InitPlayer();
void DrawPlayer();
void DrawPlayerDebug();
void UpdatePlayer(const std::vector<std::unique_ptr<Platform>>& platforms);

#endif