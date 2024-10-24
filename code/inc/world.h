#include <SDL2/SDL.h>

#include <vector>

#include "SDL_video.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "contact.h"
#include "dynamicBody.h"
#include "rigidBody.h"

struct worldStruct
{
  float gravityX;
  float gravityY;
};

class World
{
 public:
  World(worldStruct);
  ~World();

  void sdlSetup();
  void sdlRender();

  void gameLoop();

  void createWorld(worldStruct world);
  void createRigidBody(rigidStruct rigid);
  void createDynamicBody(dynamicStruct dynamic);

  void setFrictionJoins();

  b2WorldId getWorldId();

  float pixelToMeter(const float value);
  b2Vec2 pixelToMeter(const b2Vec2& rVector);
  float meterToPixel(const float value);
  b2Vec2 meterToPixel(const b2Vec2& rVector);

 private:
  float MetersPerPixelFactor = 32.0f;

  int currentCube = 0;
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;

  SDL_Window* window;
  SDL_Renderer* renderer;

  // WORLD DEFINITION
  worldStruct worldData;
  b2WorldDef worldDef;
  b2WorldId worldId;

  std::vector<RigidBody*> rigidBodies;
  std::vector<DynamicBody*> dynamicBodies;

  // GROUND BODY
  b2BodyDef groundBodyDef;
  b2BodyId groundId;
  b2Polygon groundBox;
  b2ShapeDef groundShapeDef;
  Contact contactListener;
};
