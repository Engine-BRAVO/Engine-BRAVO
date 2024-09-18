#include "ParticleMath.h"
#include "Particle.h"
#include <cmath>
#include <iostream>
#include <random>

int ParticleMath::getRandomNumber(int min, int max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(engine);
}

int ParticleMath::calculateLifespan(int lifeSpan, int lifeSpanVar) {
    std::uniform_int_distribution<int> distribution(lifeSpan - lifeSpanVar, lifeSpan + lifeSpanVar);
    return distribution(engine);
}

color ParticleMath::calculateColor(color start, color end, int life, int maxlife) {
    color tempColor;
    int redDiff = start.r - end.r;
    int greenDiff = start.g - end.g;
    int blueDiff = start.b - end.b;

    tempColor.r = end.r + redDiff / maxlife * life;
    tempColor.g = end.g + greenDiff / maxlife * life;
    tempColor.b = end.b + blueDiff / maxlife * life;

    std::cout << tempColor.r << std::endl;
    return tempColor;
}
