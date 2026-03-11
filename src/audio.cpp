#include "audio.h"
#include "al.h"
#include "alc.h"
#include <iostream>
#include <vector>

bool InitAudio() {
    // Open the default device
    ALCdevice* device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open OpenAL device" << std::endl;
        return false;
    }

    // Create context
    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!context) {
        std::cerr << "Failed to create OpenAL context" << std::endl;
        alcCloseDevice(device);
        return false;
    }

    // Make context current
    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
        return false;
    }

    return true;
}

void CloseAudio() {
    ALCcontext* context = alcGetCurrentContext();
    ALCdevice* device = alcGetContextsDevice(context);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

// Example of playing a sound
void PlaySound(const std::vector<ALubyte>& audioData, ALsizei frequency) {
    // Generate buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);

    // Fill buffer with audio data
    alBufferData(buffer, AL_FORMAT_MONO8, audioData.data(), audioData.size(), frequency);

    // Generate source
    ALuint source;
    alGenSources(1, &source);

    // Attach buffer to source
    alSourcei(source, AL_BUFFER, buffer);

    // Play source
    alSourcePlay(source);

    // Wait for sound to finish (in a real application, you'd handle this differently)
    ALint state;
    do {
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);

    // Cleanup
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}