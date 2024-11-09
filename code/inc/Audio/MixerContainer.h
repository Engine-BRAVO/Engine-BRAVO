#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class MixerContainer {
public:
    void addSound(std::string aPath, Mix_Chunk aSound);
    Mix_Chunk* getSound(std::string aIndex);
    void addMusic(Mix_Music* aMusic);
    Mix_Music* getMusic();
    void clear();

private:
    /// @brief Map of sound effects. key is the path to the sound
    std::unordered_map<std::string, Mix_Chunk> mSfx;
    Mix_Music* mMusic;
};