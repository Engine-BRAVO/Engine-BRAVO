#pragma once

#include <vector>
#include "GameObject.h"


class GameObjectSystem {
public:
    int createEntity();

    void addComponent(int, const std::string&);
    void removeComponent(int, const std::string&);

    template<typename T>
    T* getComponent(int entity) {
        const char* typeName = T().GetType();
        auto it = componentMap.find(typeName);
        if (it != componentMap.end()) {
            auto componentIt = it->second.find(entity);
            return componentIt != it->second.end() ? static_cast<T*>(componentIt->second.get()) : nullptr;
        }
        return nullptr;
    }

    void update(double);

    const std::vector<int>& getGameObjects() const;
private:
    std::vector<int> GameObjects;
    std::unordered_map<std::string, std::unordered_map<int, std::unique_ptr<IComponent>>> componentMap;
    int nextObjectId = 0;
    ComponentFactory componentFactory;
};