#pragma once

#include "GameObjectSystem.h"
#include "RenderComponent.h"

class RenderComponentSystem {
public:
    RenderComponentSystem();
    void update(GameObjectSystem&);
};