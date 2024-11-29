#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "GameObject.h"

class ParticleSystem
{
public:
	ParticleSystem();

	void update();

public:
	void addObject(GameObject& aObject);
	void removeObject(GameObject& aObject);
	const std::vector<std::reference_wrapper<GameObject>>& getObjects() const;
	void clearObjects();

private:
	std::vector<std::reference_wrapper<GameObject>> mObjects;
};

#endif // PARTICLESYSTEM_H
