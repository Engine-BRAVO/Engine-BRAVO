#pragma once

#include "Particle.h"
#include "ParticleMath.h"
#include "SDL_render.h"

#include <stdlib.h>
#include <time.h>
#include <vector>

struct emitter {
    int x;
    int y;
    int amount;
    int lifeSpan;
    int lifeSpanVar;
    color startColor;
    color endColor;
};

class ParticleSystem {
public:
    ParticleSystem(emitter);
    ~ParticleSystem();

    void updateParticles();
    std::vector<Particle> getParticles();

private:
    emitter particleEmitter;
    ParticleMath particleMath;
    std::vector<Particle> particleList;

    color m_startColor;
    color m_endColor;
};
