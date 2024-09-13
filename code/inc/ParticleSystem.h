#pragma once
#include <vector>
struct pos 
{
	int x;
	int y;
};

struct Particle 
{
	pos position;
	int life;
};


class ParticleSystem
{
public:
	ParticleSystem(int amount);
	~ParticleSystem();
private:
	int amount;
	std::vector<Particle> particleList;	
};
