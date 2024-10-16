#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"

struct rigidPos
{
  float x;
  float y;
};

struct rigidSize
{
  float width;
  float height;
};

struct rigidStruct
{
  rigidPos pos;
  rigidSize size;
};

class RigidBody
{
 public:
  RigidBody(rigidStruct rigid, b2WorldId worldId);

  b2Vec2 getRigidPos();
  rigidSize getRigidSize();

  b2BodyId getGroundid();

 private:
  rigidStruct rigidData;

  // GROUND BODY
  b2BodyDef groundBodyDef;
  b2BodyId groundId;
  b2Polygon groundBox;
  b2ShapeDef groundShapeDef;
};
