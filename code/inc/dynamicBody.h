#include "box2d/box2d.h"
#include "box2d/id.h"

struct dynamicPos
{
  float x;
  float y;
};

struct dynamicSize
{
  float width;
  float height;
};

struct dynamicProperties
{
  float density;
  float friction;
  float restitution;
};

struct dynamicStruct
{
  dynamicPos pos;
  dynamicSize size;
  dynamicProperties properties;
};

class DynamicBody
{
 public:
  DynamicBody(dynamicStruct dynamic, b2WorldId worldId);

  void createBody(b2WorldId worldId);
  void setPosition();
  dynamicPos getPosition();
  dynamicSize getSize();
  dynamicProperties getProperties();

  b2BodyId getBodyId();

 private:
  dynamicStruct dynamicData;

  // DYNAMIC BODY
  b2BodyDef bodyDef;
  b2BodyId bodyId;
  b2Polygon dynamicBox;
  b2ShapeDef shapeDef;
};
