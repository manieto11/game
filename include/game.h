#ifndef GAME_H
#define GAME_H

#define GAME_TITLE "game"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define FIXED_DELTA_TIME 0.2f

void InitGame();
void UpdateGame();
void FixedUpdateGame();
void DrawGame();
void FinishGame();

bool GameRunning();

#endif