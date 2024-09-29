#include "PhysicsComponentSystem.h"

void PhysicsComponentSystem::update(GameObjectSystem& gos, double deltaTime) {
    // Iterate through all entities and update their physics components
    for (int entity : gos.getGameObjects()) {
        IComponent* componentBase = gos.getComponent(entity, "PhysicsComponent");
        if (componentBase) {
            PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(componentBase);
            // Update physics logic here, e.g., applying forces, updating positions
            // Example: physicsComponent->velocity += physicsComponent->acceleration * deltaTime;
        }
    }
}