#pragma once

struct position 
{
	int x;
	int y;
};

class Particle
{
public:
	Particle(int x, int y);
	
	void updateParticle(int num1, int num2);
	bool checkHealth();
	position _pos;
	position _startPos;

private:
	int _life;
	int _startLife;
	int _color;

	
};
