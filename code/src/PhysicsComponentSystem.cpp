#include "PhysicsComponentSystem.h"

void PhysicsComponentSystem::update(GameObjectSystem& gos, double deltaTime) {
    // Iterate through all entities and render their components
    for (int entity : gos.getGameObjects()) {
        auto* physicsComponent = gos.getComponent<PhysicsComponent>(entity);
        if (physicsComponent) {
            // Render logic here, e.g. rendering textures
        }
    }
}