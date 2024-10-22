#include "Engine/SceneManager.h"

SceneManager::SceneManager() : mCurrentSceneIndex(-1) {}

SceneManager::~SceneManager() {}

int SceneManager::addScene(const Scene& scene) {
    mScenes.push_back(std::make_unique<Scene>(scene));
    if (mCurrentSceneIndex == -1) {
        mCurrentSceneIndex = 0;
    }
    return mScenes.size() - 1;
}

void SceneManager::loadScene(int index) {
    if (index >= 0 && index < mScenes.size()) {
        mCurrentSceneIndex = index;
    }
}

int SceneManager::getNewSceneID() {
    bool idFound = false;

    int sceneID = -1;
    while (idFound == false) {
        sceneID++;
        bool idSeen = false;
        for (int i = 0; i < mScenes.size(); i++) {
            if (mScenes[i]->getID() == sceneID) {
                idSeen = true;
            }
        }
        if (!idSeen)
            idFound = true;
    }
    return sceneID;
}

bool SceneManager::sceneNameExists(std::string aSceneName) {
    for (auto& scene : mScenes) {
        if (scene->getName() == aSceneName) {
            return true;
        }
    }
    return false;
}

bool SceneManager::sceneIDExists(int aSceneID) {
    for (auto& scene : mScenes) {
        if (scene->getID() == aSceneID) {
            return true;
        }
    }
    return false;
}

Scene* SceneManager::createScene(std::string aSceneName, int aSceneID) {
    if (sceneNameExists(aSceneName) || sceneIDExists(aSceneID)) {
        return nullptr;
    }

    if (aSceneID == -1) {
        aSceneID = getNewSceneID();
    }

    mScenes.push_back(std::make_unique<Scene>(aSceneName, aSceneID));

    return mScenes[mScenes.size() - 1].get();
}

void SceneManager::loadScene(const std::string& sceneName) {
    for (int i = 0; i < mScenes.size(); ++i) {
        if (mScenes[i]->getName() == sceneName) {
            mCurrentSceneIndex = i;
            break;
        }
    }
}

Scene* SceneManager::getCurrentScene() {
    if (mCurrentSceneIndex >= 0 && mCurrentSceneIndex < mScenes.size()) {
        return mScenes[mCurrentSceneIndex].get();
    }
    return nullptr;
}
