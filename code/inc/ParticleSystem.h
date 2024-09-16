#pragma once
#include <vector>
union pos 
{
	int x;
	int y;
};

struct Particle 
{
	int color;
	pos position;
	int life;
};


class ParticleSystem
{
public:
	ParticleSystem(int amount);
	~ParticleSystem();

	void UpdateSystem();


private:
	int _amount;
	int velocity;
	std::vector<Particle> particleList;	
};
