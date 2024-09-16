#include "ParticleSystem.h"
#include "Particle.h"
#include "SDL_render.h"

ParticleSystem::ParticleSystem(int amount, int velocity)
{
	_amount = amount;
	_velocity = velocity;
	srand (time(NULL));
	for(int i = 0; i < _amount; i++) 
	{
	particleList.push_back(Particle(100, 80));
	}
}

void ParticleSystem::updateParticles() 
{	
	
	for (std::vector<Particle>::iterator it = particleList.begin(); it != particleList.end(); ++it) 
	{
		it->checkHealth();
		it->updateParticle(getRandomNumber(), getRandomNumber());
	}
}

std::vector<Particle> ParticleSystem::getParticles() 
{
	return particleList;
}

int ParticleSystem::getRandomNumber() 
{
	return rand() % 10 + 1;	
}
