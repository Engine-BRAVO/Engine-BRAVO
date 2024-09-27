#include "world.h"

#include "box2d/types.h"

World::World() {}

World::~World() { b2DestroyWorld(worldId); }

void World::createWorld()
{
  worldDef = b2DefaultWorldDef();
  worldDef.gravity = (b2Vec2){0.0f, -10.0f};  // Configure gravity
  worldId = b2CreateWorld(&worldDef);
}

void World::createGroundBody()
{
  groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position =
      (b2Vec2){0.0f, -10.0f};  // Define a static ground body

  groundId = b2CreateBody(worldId, &groundBodyDef);
  groundBox = b2MakeBox(50.0f, 10.0f);

  groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
}

void World::createDynamicBody()
{
  bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = (b2Vec2){0.0f, 4.0f};
  bodyId = b2CreateBody(worldId, &bodyDef);

  dynamicBox = b2MakeBox(1.0f, 1.0f);

  shapeDef = b2DefaultShapeDef();
  shapeDef.density = 1.0f;
  shapeDef.friction = 0.3f;
  b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
}

b2WorldId World::getWorldId() { return worldId; }

b2BodyId World::getBodyId() { return bodyId; }
