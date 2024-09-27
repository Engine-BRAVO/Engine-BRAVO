#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
class World
{
 public:
  World();
  ~World();

  void createWorld();
  void createGroundBody();
  void createDynamicBody();

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
