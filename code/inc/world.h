#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"

struct xy
{
  float x;
  float y;
};

struct dynamicBody
{
  xy bodyPos;
  xy size;
  float density;
  float friction;
  float restitution;
};

struct rigidBody
{
  xy bodyPos;
  xy size;
};

struct worldPara
{
  xy gravity;
};

class World
{
 public:
  World();
  ~World();

  void createWorld(worldPara para);
  void createGroundBody(rigidBody body);
  void createDynamicBody(dynamicBody body);

  b2WorldId getWorldId();
  b2BodyId getBodyId();

 private:
  // WORLD DEFINITION
  b2WorldDef worldDef;
  b2WorldId worldId;

  // GROUND BODY
  b2BodyDef groundBodyDef;
  b2BodyId groundId;
  b2Polygon groundBox;
  b2ShapeDef groundShapeDef;

  // DYNAMIC BODY
  b2BodyDef bodyDef;
  b2BodyId bodyId;
  b2Polygon dynamicBox;
  b2ShapeDef shapeDef;
};
