#include "RenderComponent.h"
#include <iostream>


RenderComponent::RenderComponent() {}

const char* RenderComponent::GetType() const{
        return "RenderComponent";
}

const std::string& RenderComponent::getTexturePath() const {
    return texturePath;
}

double RenderComponent::getOpacity() const {
    return opacity;
}

void RenderComponent::setTexturePath(const std::string& Path) {
    texturePath = Path;
}

void RenderComponent::setOpacity(double Opacity) {
    opacity = Opacity;
}