/**
 * @file AudioManager.h
 *
 * @brief This file contains the declaration of the AudioManager class
 */
#pragma once

#include "AudioSource.h"
#include "IAudioFacade.h"
#include <memory>
#include <vector>

class GameObject;

/**
 * @class AudioManager
 *
 * @brief This class is responsible for owning and playing audio sources
 */
class AudioManager
{
public:
	AudioManager();

public:
	void play(const AudioSource& aSource);
	void pause(const AudioSource& aSource);
	void resume(const AudioSource& aSource);
	void stop(const AudioSource& aSource);
	void wake();

public:
	IAudioFacade& getFacade();

public:
	void loadSound(const AudioSource& aAudio);
	void clearSounds();
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	/// @brief References to all game objects with audio sources. Is regulated by calls to addObject and removeObject
	std::vector<std::reference_wrapper<GameObject>> mObjects;
	/// @brief The facade that will be used to interact with the audio system.
	std::unique_ptr<IAudioFacade> mFacade;
};