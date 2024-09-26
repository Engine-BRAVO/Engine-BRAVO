#include <iostream>
#include "GameObjectSystem.h"


int main() {
    GameObjectSystem ecs;

    int playerEntity = ecs.createEntity();
    int enemyEntity = ecs.createEntity();

    // Add components
    ecs.addComponent(playerEntity, "RenderComponent");
    ecs.addComponent(playerEntity, "PhysicsComponent");

    double deltaTime = 0.016;

    // Update systems in the ECS
    ecs.update(deltaTime);

    // Access and manipulate components
    auto* renderComponent = ecs.getComponent<RenderComponent>(playerEntity);
    if (renderComponent) {
        renderComponent->setTexturePath("empty.png");
        std::cout << "Render Texture Path: " << renderComponent->getTexturePath() << "\n";
    }

    auto* physicsComponent = ecs.getComponent<PhysicsComponent>(playerEntity);
    if (physicsComponent) {
        physicsComponent->setMass(80.0);
        std::cout << "Physics Mass: " << physicsComponent->getMass() << "\n";
    }

    // Remove components
    ecs.removeComponent(playerEntity, "RenderComponent");
    std::cout << "RenderComponent removed from playerEntity.\n";

    ecs.removeComponent(playerEntity, "PhysicsComponent");
    std::cout << "PhysicsComponent removed from playerEntity.\n";

    return 0;
}