#pragma once
#include <string>
#include <vector>

#include "Camera.h"
#include "GameObject.h"

class Scene {
    friend class SceneManager;

private:
    Scene(std::string aSceneName, int aSceneID);
    ~Scene();

public:
    void addGameObject(GameObject* object);
    void removeGameObject(int id);
    void removeGameObject(GameObject* object);
    GameObject& getGameObject(int id);
    // int addCamera();
    // void removeCamera(int id);
    // Camera& getCamera(int id);
    // void setActiveGamera(int id);
    // Camera& getActiveCamera();
    int getID();
    std::string getName();

    std::vector<GameObject*>& getGameObjects();

private:
    std::vector<GameObject*> mGameObjects; // Object of pointer?
    // std::vector<Camera> mCameras;          // Object of pointer?
    // int mActiveCameraIndex;
    std::string mSceneName;
    int mSceneID = -1;
};
