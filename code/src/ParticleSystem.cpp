#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int amount)
{
	_amount = amount;	
}

void ParticleSystem::UpdateSystem() 
{

	std::vector<Particle>::iterator iter = particleList.begin();

	for(iter; iter < particleList.end(); iter++)
	{
		iter =+ velocity; 


	}
}
