#include "RenderComponentSystem.h"

void RenderComponentSystem::update(GameObjectSystem& gos) {
    // Iterate through all entities and render their components
    for (int entity : gos.getGameObjects()) {
        auto* renderComponent = gos.getComponent<RenderComponent>(entity);
        if (renderComponent) {
            // Render logic here, e.g. rendering textures
        }
    }
}