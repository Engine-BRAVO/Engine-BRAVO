#include "box2d/box2d.h"

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

  rigidPos getRigidPos();
  rigidSize getRigidSize();

 private:
  rigidStruct rigidData;

  // GROUND BODY
  b2BodyDef groundBodyDef;
  b2BodyId groundId;
  b2Polygon groundBox;
  b2ShapeDef groundShapeDef;
};
