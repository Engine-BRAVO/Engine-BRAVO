#include "PhysicsManager.h"
#include "SceneManager.h"

PhysicsManager::PhysicsManager() {}

void PhysicsManager::startPhysicsEngine(std::vector<GameObject*>& aGameObjects, Vector2 aGravity) {

    mPhysicsEngine.setgameObjects(aGameObjects);
    mPhysicsEngine.createWorld(aGravity);
    mPhysicsEngine.createBodies();

    mPhysicsEngine.setStep(1.0f / 60.0f);
    mPhysicsEngine.setSubStep(6);
}

void PhysicsManager::updatePhysicsEngine(Scene* aScene) {
    mPhysicsEngine.updateReferences(aScene->getGameObjects());

    mPhysicsEngine.update();
}
