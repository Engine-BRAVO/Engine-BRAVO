#include <iostream>
#include "GameObjectSystem.h"


int main() {
    GameObjectSystem ecs;

    int playerEntity = ecs.createEntity();
    int enemyEntity = ecs.createEntity();

    // Add components
    if (ecs.addComponent(playerEntity, "RenderComponent")) {
        std::cout << "RenderComponent added to playerEntity.\n";
    } else {
        std::cout << "Failed to add RenderComponent to playerEntity.\n";
    }

    if (ecs.addComponent(playerEntity, "PhysicsComponent")) {
        std::cout << "PhysicsComponent added to playerEntity.\n";
    } else {
        std::cout << "Failed to add PhysicsComponent to playerEntity.\n";
    }


    double deltaTime = 0.016;

    // Update systems in the ECS
    ecs.update(deltaTime);

    // Access and manipulate components
    IComponent* renderComponentBase = ecs.getComponent(playerEntity, "RenderComponent");
    if (renderComponentBase) {
        RenderComponent* renderComponent = static_cast<RenderComponent*>(renderComponentBase);
        renderComponent->setTexturePath("empty.png");
        std::cout << "Render Texture Path: " << renderComponent->getTexturePath() << "\n";
    }


    IComponent* physicsComponentBase = ecs.getComponent(playerEntity, "PhysicsComponent");
    if (physicsComponentBase) {
        PhysicsComponent* physicsComponent = static_cast<PhysicsComponent*>(physicsComponentBase);
        physicsComponent->setMass(80.0);
        std::cout << "Physics Mass: " << physicsComponent->getMass() << "\n";
    }


    // Remove components
    if (ecs.removeComponent(playerEntity, "RenderComponent")) {
        std::cout << "RenderComponent removed from playerEntity.\n";
    } else {
        std::cout << "Failed to remove RenderComponent from playerEntity.\n";
    }

    if (ecs.removeComponent(playerEntity, "PhysicsComponent")) {
        std::cout << "PhysicsComponent removed from playerEntity.\n";
    } else {
        std::cout << "Failed to remove PhysicsComponent from playerEntity.\n";
    }


    return 0;
}