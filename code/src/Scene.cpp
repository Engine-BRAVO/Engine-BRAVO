#include "Scene.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

Scene::Scene(std::string aSceneName, int aSceneID)
    : mSceneName(std::move(aSceneName)), mSceneID(aSceneID), mActiveCameraIndex(0) {}

Scene::~Scene() = default;

void Scene::update() {
    if (mGameObjectsToRemove.size() > 0) {
        // std::cout << "Removing GameObject in the update function" << std::endl;
        for (auto& toBeRemoved : mGameObjectsToRemove) {
            removeGameObject(toBeRemoved);
        }
        mGameObjectsToRemove.clear();
    }
}

void Scene::removeGameObject(GameObject* aObject) {
    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects[i].get() == aObject) {
            mGameObjects.erase(mGameObjects.begin() + i);
            return;
        }
    }
    // std::cerr << "RemoveGameObject called but no matching object found" << std::endl;
}

std::vector<GameObject*>& Scene::getGameObjects() {
    static std::vector<GameObject*> gameObjectRefs;
    gameObjectRefs.clear();
    for (const auto& gameObject : mGameObjects) {
        gameObjectRefs.push_back(gameObject.get());
    }
    return gameObjectRefs;
}

std::vector<GameObject*> Scene::getGameObjectsWithTag(const std::string& tag) {
    std::vector<GameObject*> objectsWithTag;
    for (const auto& obj : mGameObjects) {
        if (obj->getTag() == tag) {
            objectsWithTag.push_back(obj.get());
        }
    }
    return objectsWithTag;
}

void Scene::addGameObject(GameObject* object) {
    if (object) {
        mGameObjects.push_back(std::unique_ptr<GameObject>(object));
    }
}

void Scene::requestGameObjectRemoval(int id) {
    for (const auto& obj : mGameObjects) {
        if (obj->getID() == id) {
            mGameObjectsToRemove.push_back(obj.get());
            return;
        }
    }
    throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

void Scene::requestGameObjectRemoval(GameObject* object) {
    for (const auto& obj : mGameObjects) {
        if (obj.get() == object) {
            mGameObjectsToRemove.push_back(obj.get());
            return;
        }
    }
    throw std::runtime_error("GameObject not found.");
}

GameObject& Scene::getGameObject(int id) {
    for (const auto& obj : mGameObjects) {
        if (obj->getID() == id) {
            return *obj;
        }
    }
    throw std::runtime_error("GameObject with ID " + std::to_string(id) + " not found.");
}

std::string Scene::getName() { return mSceneName; }
int Scene::getID() { return mSceneID; }

int Scene::addCamera() {
    mCameras.push_back(std::make_unique<Camera>());
    return static_cast<int>(mCameras.size() - 1); // Return the index of the newly added camera
}

void Scene::removeCamera(int id) {
    if (id >= 0 && id < static_cast<int>(mCameras.size())) {
        mCameras[id].reset(); // Delete the camera
        mCameras.erase(mCameras.begin() + id);
        // Adjust active camera if necessary
        if (mActiveCameraIndex == id) {
            mActiveCameraIndex = -1; // No active camera
        } else if (mActiveCameraIndex > id) {
            mActiveCameraIndex--;
        }
    } else {
        throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
    }
}

Camera& Scene::getCamera(int id) {
    if (id >= 0 && id < static_cast<int>(mCameras.size())) {
        return *mCameras[id];
    }
    throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
}

void Scene::setActiveCamera(int id) {
    if (id >= 0 && id < static_cast<int>(mCameras.size())) {
        mActiveCameraIndex = id;
    } else {
        throw std::runtime_error("Camera with ID " + std::to_string(id) + " not found.");
    }
}

Camera& Scene::getActiveCamera() {
    if (mActiveCameraIndex >= 0 && mActiveCameraIndex < static_cast<int>(mCameras.size())) {
        return *mCameras[mActiveCameraIndex];
    }
    throw std::runtime_error("No active camera set.");
}

void Scene::addPersistentGameObject(GameObject* object) {
    if (object) {
        mGameObjects.push_back(std::unique_ptr<GameObject>(object));
        mPersistentGameObjects.push_back(object);
    }
}

// function removes the object from the persistant gameObjects vector and from the mGameObjects vector
void Scene::removePersistentGameObject(GameObject* object) {
    for (int i = 0; i < mPersistentGameObjects.size(); i++) {
        if (mPersistentGameObjects[i] == object) {
            mPersistentGameObjects.erase(mPersistentGameObjects.begin() + i);
            break;
        }
    }

    for (int i = 0; i < mGameObjects.size(); i++) {
        if (mGameObjects[i].get() == object) {
            mGameObjects.erase(mGameObjects.begin() + i);
            break;
        }
    }
}

std::vector<GameObject*>& Scene::getPersistentGameObjects() { return mPersistentGameObjects; }

void Scene::clearPersistentGameObjects() { mPersistentGameObjects.clear(); }

void Scene::releasePersistentGameObjects() {
    for (auto& obj : mPersistentGameObjects) {
        for (int i = 0; i < mGameObjects.size(); i++) {
            if (mGameObjects[i].get() == obj) {
                GameObject* warningWeg = mGameObjects[i].release();
                mGameObjects.erase(mGameObjects.begin() + i);
                break;
            }
        }
    }
}

std::vector<GameObject*> Scene::getGameObjectsToBeRemoved() { return mGameObjectsToRemove; }
