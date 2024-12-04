#pragma once

#include <vector>

#include "Global/Color.h"
#include "Transform.h"
#include "Vector2.h"

class Particle
{
public:
	Particle(const Vector2& aPosition = Vector2(0, 0), const Vector2& aVelocity = Vector2(0, 0),
			 float aAcceleration = 0, int aLifeTime = 0, int aMaxLifeTime = 0, const Vector2& aSize = Vector2(0, 0),
			 const Vector2& aEndSize = Vector2(0, 0), float aRotation = 0, float angularVelocity = 0,
			 float angularAcceleration = 0, const std::vector<Color>& aColorGradient = std::vector<Color>());

	void update();

	Vector2 getPosition();
	float getLifeTime();
	Vector2 getSize();
	float getRotation();
	Color getColor();

private:
	Color calculateColor();
	Color calculateInterpolatedColor();
	Color getNearestColor();

private:
	Vector2 position;
	Vector2 velocity;
	float acceleration;

	int maxLifeTime;
	float lifeTimeRemainingSec;
	int initialLifeTime;

	Vector2 startSize;
	Vector2 size;
	Vector2 endSize;
	float rotation;
	float angularVelocity;
	float angularAcceleration;
	std::vector<Color> colorGradient;

	bool interpolateColor = true;
};