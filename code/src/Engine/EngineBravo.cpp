#include "EngineBravo.h"

#include <SDL.h>
#include <chrono>
#include <thread>

#include "slikenet/sleep.h"

#include "IBehaviourScript.h"
#include "Input.h"
#include "ParticleEmitter.h"
#include "Renderer.h"

EngineBravo::EngineBravo() : mFrameRateLimit(60), mRunning(false) {}

EngineBravo::~EngineBravo() {}

EngineBravo& EngineBravo::getInstance() {
    static EngineBravo instance;
    return instance;
}

void EngineBravo::initialize() {
    this->mResourceManager.setRenderer(&mRenderSystem.getRenderer());

    mConfiguration.setConfig("render_colliders", true);
    mConfiguration.setConfig("render_fps", true);

    if (mSceneManager.sceneChanged()) {
    }

    mNetworkManager.initialize();

    Time::initialize();

    mUIManager.init();

    mPhysicsManager.startPhysicsEngine(Vector2(0, 0.0f));
    return;
}

void EngineBravo::run() {
    Input& input = Input::getInstance();

    mRunning = true;

    mEventManager.subscribe([this](const Event& aEvent) { handleEvent(aEvent); }, EventType::Quit);

    mRenderSystem.getWindow().showWindow();

    while (mRunning) {
        Time::update();

        mEventManager.handleEvents();

        input.update();

        mUIManager.update(mSceneManager.getCurrentScene());

        mSceneManager.sceneChanged();

        startBehaviourScripts();

        runBehaviourScripts();

        mPhysicsManager.updatePhysicsEngine(mSceneManager.getCurrentScene());

        mParticleSystem.update(mSceneManager.getCurrentScene());

        mRenderSystem.render(mSceneManager.getCurrentScene());

        mNetworkManager.update();
        limitFrameRate(mFrameRateLimit);
    }
}

void EngineBravo::setFrameRateLimit(int aFrameRate) { mFrameRateLimit = aFrameRate; }

void EngineBravo::handleEvent(const Event& aEvent) {
    switch (aEvent.type) {
    case EventType::Quit:
        mRunning = false;
        break;
    default:
        break;
    }
}

// function that limits frame rate by keeping track of the time it takes to render a frame and delaying the next frame
// if it renders too quickly
void EngineBravo::limitFrameRate(int aFrameRate) {
    // Minimum time per frame in milliseconds
    int mMinFrameTimeMs = 1000 / aFrameRate;

    // Record the start time of the frame
    static auto frameStart = std::chrono::high_resolution_clock::now();

    // Calculate the time it took to render the frame
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto frameDurationMs = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

    // Calculate the delay needed to achieve the target frame rate
    int delayTime = mMinFrameTimeMs - frameDurationMs;

    // Delay if the frame rendered too quickly
    if (delayTime > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
    }
    frameStart = std::chrono::high_resolution_clock::now();
}

SceneManager& EngineBravo::getSceneManager() { return mSceneManager; }

RenderSystem& EngineBravo::getRenderSystem() { return mRenderSystem; }

ResourceManager& EngineBravo::getResourceManager() { return mResourceManager; }
SaveGameManager& EngineBravo::getSaveGameManager() { return mSaveGameManager; }
AudioManager& EngineBravo::getAudioManager() { return mAudioManager; }
EventManager& EngineBravo::getEventManager() { return mEventManager; }
UIManager& EngineBravo::getUIManager() { return mUIManager; }

NetworkManager& EngineBravo::getNetworkManager() { return mNetworkManager; }

Configuration& EngineBravo::getConfiguration() { return mConfiguration; }

void EngineBravo::startBehaviourScripts() {
    Scene* currentScene = mSceneManager.getCurrentScene();
    if (currentScene == nullptr) {
        std::cout << "retrieved scene is nullptr" << std::endl;
    }

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            for (auto behaviourScript : gameObject->getComponents<IBehaviourScript>()) {
                if (behaviourScript->hasScriptStarted()) {
                    continue;
                }
                behaviourScript->onStart();
                behaviourScript->setScriptStarted(true);
            }
        }
    }
}

void EngineBravo::runBehaviourScripts() {
    Scene* currentScene = mSceneManager.getCurrentScene();
    if (currentScene == nullptr) {
        std::cout << "retrieved scene is nullptr" << std::endl;
    }

    if (currentScene) {
        for (auto& gameObject : currentScene->getGameObjects()) {
            for (auto behaviourScript : gameObject->getComponents<IBehaviourScript>()) {
                behaviourScript->onUpdate();
            }
        }
    }
}

PhysicsManager& EngineBravo::getPhysicsManager() { return mPhysicsManager; }
