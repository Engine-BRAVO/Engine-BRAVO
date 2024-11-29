#ifndef ENGINEBRAVO_H
#define ENGINEBRAVO_H

#include "AnimationManager.h"
#include "AudioManager.h"
#include "Configuration.h"
#include "Engine/ResourceManager.h"
#include "Engine/SaveGameManager.h"
#include "EventManager.h"
#include "Network/NetworkManager.h"
#include "ParticleSystem.h"
#include "PhysicsManager.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Time.h"
#include "UIManager.h"
#include "UpdateQueue.h"
#include <queue>

class EngineBravo
{
public:
	// Get the single instance of EngineBravo
	static EngineBravo& getInstance();

	void initialize();
	void run();

	void setFrameRateLimit(int aFrameRate);

	SceneManager& getSceneManager();
	RenderSystem& getRenderSystem();
	ResourceManager& getResourceManager();
	ParticleSystem& getParticleSystem();
	NetworkManager& getNetworkManager();
	SaveGameManager& getSaveGameManager();
	AudioManager& getAudioManager();
	EventManager& getEventManager();
	UIManager& getUIManager();

	Configuration& getConfiguration();
	// UIManager& getUIManager();
	PhysicsManager& getPhysicsManager();

	UpdateQueue& getUpdateQueue();

private:
	// Private constructor and destructor
	EngineBravo();
	~EngineBravo();

	// Deleted copy constructor and assignment operator to prevent copying
	EngineBravo(const EngineBravo&) = delete;
	EngineBravo& operator=(const EngineBravo&) = delete;

	void runBehaviourScripts();
	void startBehaviourScripts();

	void limitFrameRate(int aFrameRate);

	void handleEvent(const Event& aEvent);

private:
	int mFrameRateLimit;
	bool mRunning;

	SceneManager mSceneManager;
	RenderSystem mRenderSystem;
	ResourceManager mResourceManager;
	ParticleSystem mParticleSystem;
	NetworkManager mNetworkManager;
	EventManager mEventManager;
	SaveGameManager mSaveGameManager;
	AudioManager mAudioManager;
	UIManager mUIManager;

	Configuration mConfiguration;

	PhysicsManager mPhysicsManager;
	// AnimationManager animationManager;

	UpdateQueue mUpdateQueue;
};

#endif // ENGINEBRAVO_H
