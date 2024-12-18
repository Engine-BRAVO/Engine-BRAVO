#pragma once

#include "IAudioFacade.h"
#include "MixerContainer.h"
#include <SDL_mixer.h>

class MixerFacade : public IAudioFacade {
public:
    MixerFacade();

private:
    void loadSound(const std::string& aPath) override;
    void loadMusic(const std::string& aPath) override;
    void unloadAll() override;
    bool audioIsLoaded(const std::string& aPath) const override;
    bool musicIsLoaded() const override;

    void playSound(std::string aPath, bool aLooping, unsigned aVolume, int aDirection) override;
    void playMusic(int aVolume) override;
    void stopMusic() override;

    bool isPlaying(const std::string& aPath) const override;

    int distanceToAngle(int aDirection) const;
    int findAvailableChannel();

private:
    const unsigned mChannelCount{MIX_CHANNELS};
    unsigned mLastUsedChannel; // to help with finding the next available channel
    MixerContainer mMixerContainer;
};