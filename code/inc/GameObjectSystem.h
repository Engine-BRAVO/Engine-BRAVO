#pragma once

#include <vector>
#include "GameObject.h"


class GameObjectSystem {
public:
    int createEntity();

    bool addComponent(int, const std::string&);
    bool removeComponent(int, const std::string&);

    IComponent* getComponent(int, const std::string&);

    void update(double);

    const std::vector<int>& getGameObjects() const;
private:
    std::vector<int> GameObjects;
    std::unordered_map<std::string, std::unordered_map<int, std::unique_ptr<IComponent>>> componentMap;
    int nextObjectId = 0;
    ComponentFactory componentFactory;
};