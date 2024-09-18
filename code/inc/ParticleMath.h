#pragma once
#include "Particle.h"
#include <random>

class ParticleMath {
public:
    ParticleMath() : engine(std::random_device{}()){};

    int getRandomNumber(int min, int max);
    int calculateLifespan(int maxLife, int lifeVar);
    color calculateColor(color startColor, color endColor, int life, int maxlife);

private:
    std::mt19937 engine;
};
