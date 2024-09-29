#include "GameObjectSystem.h"
#include "IComponent.h"
#include "ComponentFactory.h"

int GameObjectSystem::createEntity() {
    int objectId = nextObjectId++;
    GameObjects.push_back(objectId);
    return objectId; // Return the new entity ID
}

bool GameObjectSystem::addComponent(int entity, const std::string& type) {
    std::unique_ptr<IComponent> component = ComponentFactory::createComponent(type);
    if (component) {
        componentMap[type][entity] = std::move(component); // Use std::move to transfer ownership
        return true;
    }
    return false;
}

bool GameObjectSystem::removeComponent(int entity, const std::string& type) {
    auto it = componentMap.find(type);
    if (it != componentMap.end()) {
        it->second.erase(entity); // Remove the entity from the component map
        // If the map for the type becomes empty, you can also consider erasing the type from the map:
        if (it->second.empty()) {
            componentMap.erase(it); // Remove the component type if no entities have it
        }
        return true;
    }
    return false;
}

IComponent* GameObjectSystem::getComponent(int entity, const std::string& typeName) {
    auto it = componentMap.find(typeName);
    if (it != componentMap.end()) {
        auto componentIt = it->second.find(entity);
        return componentIt != it->second.end() ? componentIt->second.get() : nullptr;
    }
    return nullptr;
}

void GameObjectSystem::update(double deltaTime) {
    
}

const std::vector<int>& GameObjectSystem::getGameObjects() const {
    return GameObjects; 
}