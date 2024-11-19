#include "AudioManager.h"
#include "EngineBravo.h"
#include <gtest/gtest.h>

class AudioTest : public ::testing::Test {
protected:
    EngineBravo* engine;
    AudioManager* audioManager;

    void SetUp() override {
        engine = &EngineBravo::getInstance();
        audioManager = &engine->getAudioManager();
    }
};

TEST_F(AudioTest, AddSound) {
    // Add a sound to the audio manager
    AudioSource audio("Audio/gun1.wav", false);

    // Verify that the sound is not loaded
    ASSERT_FALSE(audioManager->getFacade().audioIsLoaded("Audio/gun1.wav"));

    // Load the sound
    audioManager->loadSound(audio);

    // Verify that the sound was added
    ASSERT_TRUE(audioManager->getFacade().audioIsLoaded("Audio/gun1.wav"));
}

TEST_F(AudioTest, AddMusic) {
    // Add a music file to the audio manager
    AudioSource audio("Audio/music.wav", true);

    // Verify that the music is not loaded
    ASSERT_FALSE(audioManager->getFacade().musicIsLoaded());

    // Load the music
    audioManager->loadSound(audio);

    // Verify that the music was added
    ASSERT_TRUE(audioManager->getFacade().musicIsLoaded());
}

TEST_F(AudioTest, PlaySound) {
    // Add a sound to the audio manager
    std::string path = "Audio/gun1.wav";
    AudioSource audio(path, false);
    audioManager->loadSound(audio);

    // Play the sound
    audioManager->play(audio);

    // Verify that the sound is playing
    ASSERT_TRUE(audioManager->getFacade().isPlaying(path));
}

TEST_F(AudioTest, PlayMusic) {
    // Add a music file to the audio manager
    std::string path = "Audio/music.wav";
    AudioSource audio(path, true);
    audioManager->loadSound(audio);

    // Play the music
    audioManager->play(audio);

    // Verify that the music is playing
    ASSERT_TRUE(audioManager->getFacade().isMusicPlaying());
}