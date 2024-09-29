#include "RenderComponentSystem.h"

void RenderComponentSystem::update(GameObjectSystem& gos) {
    // Iterate through all entities and render their components
    for (int entity : gos.getGameObjects()) {
        IComponent* componentBase = gos.getComponent(entity, "RenderComponent");
        if (componentBase) {
            RenderComponent* renderComponent = static_cast<RenderComponent*>(componentBase);
            // Render logic here, e.g., rendering textures
        }
    }
}