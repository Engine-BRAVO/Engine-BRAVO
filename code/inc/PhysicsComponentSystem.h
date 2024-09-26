#pragma once
#include "PhysicsComponent.h"
#include "GameObjectSystem.h"


class PhysicsComponentSystem {
public:
    PhysicsComponentSystem();
    void update(GameObjectSystem&, double);
};