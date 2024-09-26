#include "GameObject.h"
#include <algorithm> 

GameObject::GameObject() 
    : name(name), id(id) {}



const std::string& GameObject::getName() const {
    return name;
}



int GameObject::getId() const {
    return id;
}


// RenderComponent* GameObject::getRenderComponent() {
//     for (const auto& component : components) {
//         if (auto renderComponent = dynamic_cast<RenderComponent*>(component.get())) {
//             return renderComponent; // Return pointer to the RenderComponent
//         }
//     }
//     return nullptr;
// }

// PhysicsComponent* GameObject::getPhysicsComponent() {
//     for (const auto& component : components) {
//         if (auto physicsComponent = dynamic_cast<PhysicsComponent*>(component.get())) {
//             return physicsComponent; // Return pointer to the PhysicsComponent
//         }
//     }
//     return nullptr;
// }
