#include "rigidBody.h"

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"

RigidBody::RigidBody(rigidStruct rigid, b2WorldId worldId) : rigidData(rigid)
{
  groundBodyDef = b2DefaultBodyDef();
  groundBodyDef.position = (b2Vec2){
      rigidData.pos.x, rigidData.pos.y};  // Define a static ground body

  groundId = b2CreateBody(worldId, &groundBodyDef);
  groundBox = b2MakeBox(rigidData.size.width, rigidData.size.height);

  groundShapeDef = b2DefaultShapeDef();
  b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);
}

b2BodyId RigidBody::getGroundid() { return this->groundId; }

b2Vec2 RigidBody::getRigidPos() { return b2Body_GetPosition(groundId); }
rigidSize RigidBody::getRigidSize() { return rigidData.size; }
