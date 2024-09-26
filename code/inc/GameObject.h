#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "IComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "ComponentFactory.h"

class GameObject {
public:
    GameObject();

    
    //Getters
    const std::string& getName() const;
    int getId() const;
    
    //Setters

private:
    std::string name;
    int id;
};