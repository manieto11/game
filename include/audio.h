#ifndef AUDIO_H
#define AUDIO_H

#include "al.h"
#include <vector>

bool InitAudio();
void CloseAudio();
void PlaySound(const std::vector<ALubyte>& audioData, ALsizei frequency);

#endif