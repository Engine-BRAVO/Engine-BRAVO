/**
 * @file LevelBuilder.cpp
 * @brief This file contains the implementation of the LevelBuilder class
 */

#include "LevelBuilder.h"
#include "AudioSource.h"
#include "BoxCollider.h"
#include "EngineBravo.h"
#include "RigidBody.h"
#include "Scene.h"
#include "Sprite.h"

/**
 * @brief This function creates a level from the parsed json data
 *
 * @param scene
 * @param tileMapData
 * @throw runtime_error if scene is null
 */
void LevelBuilder::createLevel(Scene* scene, const TileMapData& tileMapData)
{
	if (scene == nullptr)
	{
		throw std::runtime_error("Scene is null in LevelBuilder::createLevel");
	}

	EngineBravo& engine = EngineBravo::getInstance();

	for (const auto& mapObject : tileMapData.mMapObjects)
	{
		if (!mapObject.type.empty())
		{
			std::string type = mapObject.type;
			if (type == "room_entry")
			{
				createRoomEntry(scene, mapObject, tileMapData);
			}
			else if (type == "LevelEndTrigger")
			{
				createLevelEndTrigger(scene, mapObject);
			}
		}
	}

	GameObject* musicObject = new GameObject;
	AudioSource* music = new AudioSource("Audio/music.wav", true);
	music->setPlayOnWake(true);
	music->setVolume(10);
	music->setXDirection(0);
	musicObject->addComponent(music);
	scene->addGameObject(musicObject);

	createTileLayers(scene, tileMapData);
}

/**
 * @brief This function creates the room entry triggers
 *
 * @param scene
 * @param mapObject
 * @param tileMapData
 */
void LevelBuilder::createRoomEntry(Scene* scene, const MapObject& mapObject, const TileMapData& tileMapData) const
{
	std::vector<MapObject> enemySpawns;
	for (const auto& spawnPoint : tileMapData.mMapObjects)
	{
		if (spawnPoint.properties.find("isEnemySpawn") != spawnPoint.properties.end() &&
			spawnPoint.properties.at("isEnemySpawn") == "true" &&
			spawnPoint.properties.at("roomID") == mapObject.properties.at("roomID"))
		{
			enemySpawns.push_back(spawnPoint);
		}
	}

	GameObject* roomObject = new GameObject;
	roomObject->setName("RoomTrigger" + mapObject.properties.at("roomID"));
	roomObject->setTag("RoomTrigger");

	addTriggerCollider(roomObject, mapObject);

	scene->addGameObject(roomObject);
}

/**
 * @brief This function creates the level end trigger
 *
 * @param scene
 * @param mapObject
 */
void LevelBuilder::createLevelEndTrigger(Scene* scene, const MapObject& mapObject) const
{
	GameObject* levelEndObject = new GameObject;
	levelEndObject->setName("LevelEndTrigger");
	levelEndObject->setTag("LevelEnd");

	addTriggerCollider(levelEndObject, mapObject);

	scene->addGameObject(levelEndObject);
}

/**
 * @brief This function adds a collider and rigidbody to a trigger game object (Like room entry or level end)
 *
 * @param gameObject
 * @param mapObject
 */
void LevelBuilder::addTriggerCollider(GameObject* gameObject, const MapObject& mapObject) const
{
	BoxCollider* boxCollider = new BoxCollider();
	Transform transform;
	transform.position.x = mapObject.x;
	transform.position.y = mapObject.y;
	boxCollider->setTransform(transform);
	boxCollider->setWidth(mapObject.width);
	boxCollider->setHeight(mapObject.height);
	boxCollider->setTrigger(true);
	gameObject->addComponent(boxCollider);

	RigidBody* rigidBody = new RigidBody();
	rigidBody->setTransform(transform);
	gameObject->addComponent(rigidBody);
}

/**
 * @brief This function creates the tile layers
 *
 * @param scene
 * @param tileMapData
 * @throw runtime_error if tile not found in tileInfoMap
 */
void LevelBuilder::createTileLayers(Scene* scene, const TileMapData& tileMapData) const
{
	for (size_t layerIndex = 0; layerIndex < tileMapData.mLayers.size(); ++layerIndex)
	{
		bool isDoorsLayer = (tileMapData.mLayerNames[layerIndex] == "Doors");
		bool isGraphLayer = (tileMapData.mLayerNames[layerIndex] == "Graph");

		for (size_t rowIndex = 0; rowIndex < tileMapData.mLayers[layerIndex].size(); ++rowIndex)
		{
			for (size_t colIndex = 0; colIndex < tileMapData.mLayers[layerIndex][rowIndex].size(); ++colIndex)
			{
				int tile = tileMapData.mLayers[layerIndex][rowIndex][colIndex];
				if (tile != 0)
				{
					auto it = tileMapData.mTileInfoMap.find(tile);
					if (it != tileMapData.mTileInfoMap.end())
					{
						createTile(scene, it->second, layerIndex, rowIndex, colIndex, isDoorsLayer, isGraphLayer);
					}
					else
					{
						std::runtime_error("Tile not found in tileInfoMap in LevelBuilder::createTileLayers");
					}
				}
			}
		}
	}
}

/**
 * @brief This function creates a specific tile, and is called from createTileLayers
 *
 * @param scene
 * @param tileInfo
 * @param layerIndex
 * @param rowIndex
 * @param colIndex
 * @param isDoorsLayer
 * @param isGraphLayer
 */
void LevelBuilder::createTile(Scene* scene, const TileInfo& tileInfo, int layerIndex, int rowIndex, int colIndex,
							  bool isDoorsLayer, bool isGraphLayer) const
{
	EngineBravo& engine = EngineBravo::getInstance();

	SpriteDef spriteDef = {tileInfo.mTilesetName,
						   Rect{tileInfo.mCoordinates.first, tileInfo.mCoordinates.second, 16, 16}, 16, 16};

	GameObject* gameObject = new GameObject;

	Transform objectTransform;
	objectTransform.position.x = static_cast<int>(colIndex * 16);
	objectTransform.position.y = static_cast<int>(rowIndex * 16);
	gameObject->setTransform(objectTransform);

	if (!isGraphLayer)
	{
		Sprite* sprite = engine.getResourceManager().createSprite(spriteDef);
		sprite->setLayer(layerIndex);
		gameObject->addComponent(sprite);
	}

	for (const auto& collider : tileInfo.mColliders)
	{
		BoxCollider* boxCollider = new BoxCollider();
		Transform transform;
		transform.position.x = collider.x;
		transform.position.y = collider.y;
		boxCollider->setTransform(transform);
		boxCollider->setWidth(collider.mWidth + 0.1f);
		boxCollider->setHeight(collider.mHeight + 0.1f);
		boxCollider->setCollideCategory(1);
		boxCollider->setCollideWithCategory({1, 2, 3});
		if (isDoorsLayer)
		{
			boxCollider->setActive(false);
		}
		gameObject->addComponent(boxCollider);
	}

	if (!tileInfo.mColliders.empty())
	{
		RigidBody* rigidBody = new RigidBody();
		rigidBody->setTransform(objectTransform);
		if (isDoorsLayer)
		{
			if (rigidBody != nullptr)
			{
				rigidBody->setActive(false);
			}
		}
		rigidBody->setFriction(1.0f);
		gameObject->addComponent(rigidBody);
		gameObject->setName("Tile");
	}
	if (isDoorsLayer)
	{
		gameObject->setTag("Door");
	}
	scene->addGameObject(gameObject);
}