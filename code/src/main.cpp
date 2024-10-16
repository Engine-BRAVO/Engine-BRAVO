#include <SDL2/SDL.h>

#include "world.h"

int main(int argc, char* argv[])
{
  worldStruct worldData = {0.0f, 0.0f};
  World* world = new World(worldData);

  dynamicStruct dynamicData{
      .pos = {.x = 0.0f, .y = 100.0f},
      .size = {.width = 30.0f, .height = 30.0f},
      .properties = {.density = 1.0f, .friction = 0.8f, .restitution = 0.0f}};

  dynamicStruct dynamicData2{
      .pos = {.x = 0.0f, .y = 300.0f},
      .size = {.width = 30.0f, .height = 30.0f},
      .properties = {.density = 1.0f, .friction = 0.0f, .restitution = 0.0f}};

  rigidStruct rigidData{.pos = {.x = 300.0f, .y = 300.0f},
                        .size = {.width = 200.0f, .height = 50.0f}};
  // rigidStruct rigidData2{.pos = {.x = 300.0f, .y = 500.0f},
  //                      .size = {.width = 100.0f, .height = 100.0f}};

  world->createDynamicBody(dynamicData);
  world->createDynamicBody(dynamicData2);
  world->createRigidBody(rigidData);

  // world->createRigidBody(rigidData2);

  world->sdlSetup();
  while (true)
  {
    world->gameLoop();
  }
  return 0;
}
