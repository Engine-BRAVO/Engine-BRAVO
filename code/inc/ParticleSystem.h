#pragma once

#include "Particle.h"
#include "SDL_render.h"

#include <stdlib.h>
#include <vector>
#include <time.h>

class ParticleSystem
{
public:
	ParticleSystem(int amount, int velocity);
	~ParticleSystem();
	
	void updateParticles();
	std::vector<Particle> getParticles();
	int getRandomNumber();

private:
	int _amount;
	int _velocity;

	std::vector<Particle> particleList;
};
