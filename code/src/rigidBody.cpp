#include "rigidBody.h"

#include "box2d/id.h"

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

rigidPos RigidBody::getRigidPos() { return rigidData.pos; }
rigidSize RigidBody::getRigidSize() { return rigidData.size; }
