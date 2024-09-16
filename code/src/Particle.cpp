#include "Particle.h"

Particle::Particle(int x, int y)
{
	_pos.x = x;
	_pos.y = y;

	_startPos.x = x;
	_startPos.y = y;
	
	_startLife = 20;
	_life = 20;

}

void Particle::updateParticle(int num1, int num2) 
{
	_life--;
	_pos.y -= num1;
	_pos.x -= num2;
}

bool Particle::checkHealth()
{
	if (_life <= 0)
	{
		_pos.x = _startPos.x;
		_pos.y = _startPos.y;
		_life = _startLife;
		return false;
	}
	else
		return true;
}
