#include "ParticleSystem.h"
#include "Particle.h"
#include "SDL_render.h"

ParticleSystem::ParticleSystem(emitter emitterSettings) {
    particleEmitter = emitterSettings;
    srand(time(NULL));

    for (int i = 0; i < particleEmitter.amount; i++) {
        particleList.push_back(
            Particle(particleEmitter.y, particleEmitter.x,
                     particleMath.calculateLifespan(particleEmitter.lifeSpan, particleEmitter.lifeSpanVar),
                     particleEmitter.startColor, particleEmitter.endColor));
    }
}

void ParticleSystem::updateParticles() {

    for (std::vector<Particle>::iterator it = particleList.begin(); it != particleList.end(); ++it) {
        it->checkHealth();
        it->updateParticle(particleMath.getRandomNumber(0, 10), particleMath.getRandomNumber(0, 10));
        it->setColor(particleMath.calculateColor(particleEmitter.startColor, particleEmitter.endColor, it->getHealth(),
                                                 it->_startLife));
    }
}

std::vector<Particle> ParticleSystem::getParticles() { return particleList; }
