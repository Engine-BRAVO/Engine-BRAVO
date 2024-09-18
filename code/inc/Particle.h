#pragma once

struct position {
    int x;
    int y;
};

struct color {
    int r;
    int g;
    int b;
};

class Particle {
public:
    Particle(int x, int y, int life, color start, color end);

    void updateParticle(int num1, int num2);
    void setColor(color currentColor);
    bool checkHealth();
    position _pos;
    position _startPos;
    color getColor();
    int getHealth();
    int _startLife;

private:
    int _life;
    int green;
    color m_currentColor;
    color m_startColor;
    color m_endColor;
};
