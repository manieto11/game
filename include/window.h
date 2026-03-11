#ifndef WINDOW_H
#define WINDOW_H

bool InitWindow(int width, int height, const char* name);
bool WindowShouldClose();
void UpdateWindow();
void CloseWindow();

#endif