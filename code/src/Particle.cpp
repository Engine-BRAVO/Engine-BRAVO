#include "Particle.h"
#include "ParticleMath.h"
#include <stdlib.h>
#include <time.h>

Particle::Particle(int x, int y, int life, color startColor, color endColor) {
    _pos.x = x;
    _pos.y = y;

    _startPos.x = x;
    _startPos.y = y;

    _startLife = life;
    _life = life;
}

void Particle::updateParticle(int num1, int num2) {
    _life -= num1;
    _pos.y += num1;
    _pos.x -= num2;
}

bool Particle::checkHealth() {
    if (_life <= 0) {
        _pos.x = _startPos.x + rand() % 1 + 1;
        _pos.y = _startPos.y - rand() % 1 + 1;
        _life = _startLife;
        m_currentColor = m_startColor;

        m_currentColor = m_startColor;

        return false;
    } else
        return true;
}

color Particle::getColor() { return m_currentColor; }

int Particle::getHealth() { return _life; }

void Particle::setColor(color tempColor) { m_currentColor = tempColor; }
