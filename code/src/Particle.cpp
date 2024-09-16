#include "Particle.h"
#include <time.h>
#include <stdlib.h>

Particle::Particle(int x, int y)
{
	_pos.x = x;
	_pos.y = y;

	_startPos.x = x;
	_startPos.y = y;
	
	_startLife = 50;
	_life = 50;

}

void Particle::updateParticle(int num1, int num2) 
{
	_life -= num1;
	_pos.y -= num1;
	_pos.x -= num2;
}

bool Particle::checkHealth()
{
	if (_life <= 0)
	{
		_pos.x = _startPos.x + rand() % 1 + 1;
		_pos.y = _startPos.y - rand() % 1 + 1;
		_life = _startLife;
		return false;
	}
	else
		return true;
}
