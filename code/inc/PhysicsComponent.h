#pragma once
#include "IComponent.h"
#include <iostream>

class PhysicsComponent : public IComponent {
    public:
        PhysicsComponent();
        const char* GetType() const;

        double getMass() const;
        double getFriction() const;

        void setMass(double);
        void setFriction(double);
    private:
        double mass;
        double friction;
};