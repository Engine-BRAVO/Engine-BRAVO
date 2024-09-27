#include "world.h"

#include "box2d/types.h"

World::World() {}

World::~World() { b2DestroyWorld(worldId); }

void World::createWorld(worldPara para)
{
  worldDef = b2DefaultWorldDef();
  worldDef.gravity =
      (b2Vec2){para.gravity.x, para.gravity.y};  // Configure gravity
  worldId = b2CreateWorld(&worldDef);
}

void World::createGroundBody(rigidBody body)
{
  groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position =
      (b2Vec2){body.bodyPos.x,
               body.bodyPos.y + body.size.y};  // Define a static ground body

  groundId = b2CreateBody(worldId, &groundBodyDef);
  groundBox = b2MakeBox(body.size.x, body.size.y);

  groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
}

void World::createDynamicBody(dynamicBody body)
{
  bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = (b2Vec2){body.bodyPos.x,
                              body.bodyPos.y};  // Initial position above ground
  bodyId = b2CreateBody(worldId, &bodyDef);

  // Create a dynamic box shape (1 unit x 1 unit)
  dynamicBox = b2MakeBox(body.size.x, body.size.y);

  shapeDef = b2DefaultShapeDef();
  shapeDef.density = body.density;    // Density to affect the bounce
  shapeDef.friction = body.friction;  // Low friction to allow sliding
  shapeDef.restitution =
      body.restitution;  // Bounciness (value between 0 and 1)

  b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
}

b2WorldId World::getWorldId() { return worldId; }

b2BodyId World::getBodyId() { return bodyId; }
