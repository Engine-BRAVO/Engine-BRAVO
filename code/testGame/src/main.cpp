#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "EngineBravo.h"

#include "Network/NetworkTransform.h"
#include "NetworkBehaviourScript.h"
#include "PlayerPrefab.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    engine.initialize();
    SceneManager& sceneManager = engine.getSceneManager();

    NetworkManager& networkManager = engine.getNetworkManager();

    GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

    networkManager.setDefaultPlayerPrefab(defaultPlayerPrefab);

    Scene* scene = sceneManager.createScene("networkScene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(100, 100)));
    scene->getActiveCamera().setWidth(200);
    scene->getActiveCamera().setHeight(200);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<NetworkBehaviourScript>();

    scene->addGameObject(gameObject);

    sceneManager.requestSceneChange("networkScene");

    engine.run();

    return;
}

int main() {
    engineTest();

    return 0;
}