#pragma once

#include "AnimationManager.h"
#include "ResourceManager.h"
#include "SaveGameManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Renderer.h"

class EngineBravo {
public:
    EngineBravo();
    ~EngineBravo();
    void initizalize();
    void run();
    SceneManager& getSceneManager();
    void runBehaviourScripts();

private:
    SceneManager sceneManager;
    Renderer renderer;
    // SaveGameManager saveGameManager;
    // ResourceManager resourceManager;
    // AnimationManager animationManager;
    // UIManager uiManager;
};
