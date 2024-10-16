#include "dynamicBody.h"

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"

DynamicBody::DynamicBody(dynamicStruct dynamic, b2WorldId worldId)
    : dynamicData(dynamic)
{
  bodyDef = b2DefaultBodyDef();
  bodyDef.type = b2_dynamicBody;
  bodyDef.position = (b2Vec2){
      dynamicData.pos.x, dynamicData.pos.y};  // Initial position above ground
  bodyDef.fixedRotation = true;
  bodyDef.linearDamping = 0.1f;
  bodyDef.angularDamping = 0.1f;

  bodyId = b2CreateBody(worldId, &bodyDef);

  // Create a dynamic box shape (1 unit x 1 unit)
  dynamicBox = b2MakeBox(dynamicData.size.width, dynamicData.size.height);

  shapeDef = b2DefaultShapeDef();
  shapeDef.density = dynamicData.properties.density;
  shapeDef.friction = dynamicData.properties.friction;
  shapeDef.restitution = dynamicData.properties.restitution;
  b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
  b2Body_EnableSleep(bodyId, false);
}

void DynamicBody::setPosition()
{
  b2Vec2 position = b2Body_GetPosition(bodyId);
  dynamicData.pos = {position.x, position.y};
}

b2Vec2 DynamicBody::getPosition() { return b2Body_GetPosition(bodyId); }
dynamicSize DynamicBody::getSize() { return dynamicData.size; }
b2BodyId DynamicBody::getBodyId() { return bodyId; }
