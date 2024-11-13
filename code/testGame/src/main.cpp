#include <string>
#include <vector>

#include "Animation.h"
#include "EngineBravo.h"
#include "FSConverter.h"
#include "GameObject.h"
#include "InitBehaviourScript.h"
#include "PlayerBehaviourScript.h"
#include "Renderer.h"
#include "SampleBevahiourScript.h"
#include "SaveGameManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Sprite.h"
#include "SpriteAtlas.h"
#include "SpriteDef.h"
#include "TestBehaviourScript.h"
#include "Texture.h"
#include "TileMapParser.h"
#include "Transform.h"
#include "Window.h"

void engineTest() {
    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();

    Scene* scene = sceneManager.createScene("initscene");
    if (scene == nullptr)
        exit(1);

    int cameraID = scene->addCamera();
    scene->setActiveCamera(cameraID);

    scene->getActiveCamera().setTransform(Transform(Vector2(80, 0)));
    scene->getActiveCamera().setWidth(160);
    scene->getActiveCamera().setHeight(90);

    GameObject* gameObject = new GameObject;

    gameObject->addComponent<InitBehaviourScript>();

    scene->addGameObject(gameObject);

    // GameObject* testObject = new GameObject;
    //
    // testObject->addComponent<TestBehaviourScript>();
    //
    // scene->addPersistentGameObject(testObject);

    sceneManager.requestSceneChange("initscene");

    engine.initizalize();
    engine.run();

    return;
}

int main() {
    engineTest();
    return 0;
}
