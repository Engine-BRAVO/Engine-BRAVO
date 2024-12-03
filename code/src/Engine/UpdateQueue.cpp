/**
 * @file UpdateQueue.cpp
 *
 * @brief Contains the implementation of the UpdateQueue class
 */
#include "UpdateQueue.h"

#include "Animation.h"
#include "EngineBravo.h"
#include "GameObject.h"
#include "NetworkObject.h"
#include "ParticleEmitter.h"
#include "Text.h"

/**
 * @brief Adds a game object to the list of objects to be updated. If the object is already in the list, it is not added
 * again.
 *
 * @param aGameObject The game object to be added
 */
void UpdateQueue::addToUpdateObjects(GameObject& aGameObject)
{
	std::cout << "Current scene name: " << EngineBravo::getInstance().getSceneManager().getCurrentScene()->getName()
			  << std::endl;
	auto currObjects = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjects();
	auto currObjectsIt = std::find(currObjects.begin(), currObjects.end(), &aGameObject);
	if (currObjectsIt == currObjects.end())
	{
		// Object is not in the scene, do not add it
		return;
	}

	auto it = std::find_if(mUpdateObjects.begin(), mUpdateObjects.end(),
						   [&aGameObject](const std::reference_wrapper<GameObject>& wrapper)
						   {
							   return &wrapper.get() == &aGameObject; // Compare addresses
						   });
	if (it == mUpdateObjects.end())
	{
		// Object is not in the list, add it
		mUpdateObjects.push_back(aGameObject);
	}
}

/**
 * @brief Clears the list of objects to be updated
 */
void UpdateQueue::clearUpdateObjects() { mUpdateObjects.clear(); }

/**
 * @brief Updates the managers with the objects that have been added. Removes the object from each manager, then checks
 * if the manager requires it, and if so, it is added.
 */
void UpdateQueue::updateAdditions()
{
	// Iterate through the objects and update each manager
	for (GameObject& gameObject : mUpdateObjects)
	{
		// Scene manager: does not use a list of game objects
		// Render system
		EngineBravo::getInstance().getRenderSystem().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Text))
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Animation>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<Sprite>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			EngineBravo::getInstance().getRenderSystem().addObject(gameObject);
		}
		// Resource manager: does not use a list of game objects
		// Particle system
		EngineBravo::getInstance().getParticleSystem().removeObject(gameObject);
		if (gameObject.hasComponent<ParticleEmitter>())
		{
			EngineBravo::getInstance().getParticleSystem().addObject(gameObject);
		}
		// Network manager:
		EngineBravo::getInstance().getNetworkManager().removeObject(gameObject);
		if (gameObject.hasComponent<NetworkObject>())
		{
			EngineBravo::getInstance().getNetworkManager().addObject(gameObject);
		}
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		EngineBravo::getInstance().getAudioManager().removeObject(gameObject);
		if (gameObject.hasComponent<AudioSource>())
		{
			EngineBravo::getInstance().getAudioManager().addObject(gameObject);
		}
		// UI manager:
		EngineBravo::getInstance().getUIManager().removeObject(gameObject);
		if (typeid(gameObject) == typeid(Button))
		{
			EngineBravo::getInstance().getUIManager().addObject(gameObject);
		}
		// Physics manager
		EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().removeObject(gameObject);
		if (gameObject.hasComponent<RigidBody>())
		{
			EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
		if (gameObject.hasComponent<BoxCollider>())
		{
			EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().addObject(gameObject);
		}
	}
	mUpdateObjects.clear();
}

/**
 * @brief Updates the managers with the objects that have been removed. Removes the object from each manager. Does this
 * based on the objects to be removed list from the current scene.
 */
void UpdateQueue::updateRemovals()
{
	for (GameObject* gameObject :
		 EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjectsToBeRemoved())
	{
		// Scene manager: does not use a list of game objects
		// Render system
		EngineBravo::getInstance().getRenderSystem().removeObject(*gameObject);
		// Resource manager: does not use a list of game objects
		// Particle system
		EngineBravo::getInstance().getParticleSystem().removeObject(*gameObject);
		// Network manager:
		EngineBravo::getInstance().getNetworkManager().removeObject(*gameObject);
		// Event manager: does not use a list of game objects
		// Save game manager: does not use a list of game objects
		// Audio manager
		EngineBravo::getInstance().getAudioManager().removeObject(*gameObject);
		// UI manager
		EngineBravo::getInstance().getUIManager().removeObject(*gameObject);
		// Physics manager
		EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().removeObject(*gameObject);
	}
}

/**
 * @brief Clears the objects from each manager, by calling the clearObjects method for each manager.
 */
void UpdateQueue::clearManagerObjects()
{
	EngineBravo::getInstance().getRenderSystem().clearObjects();
	EngineBravo::getInstance().getParticleSystem().clearObjects();
	EngineBravo::getInstance().getNetworkManager().clearObjects();
	EngineBravo::getInstance().getAudioManager().clearObjects();
	EngineBravo::getInstance().getUIManager().clearObjects();
	EngineBravo::getInstance().getPhysicsManager().getPhysicsEngine().clearObjects();
}