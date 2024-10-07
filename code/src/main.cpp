#include <SDL2/SDL.h>

#include "world.h"

int main(int argc, char* argv[])
{
  worldStruct worldData = {0.0f, 1000.0f};
  World* world = new World(worldData);

  dynamicStruct dynamicData{
      .pos = {.x = 300.0f, .y = 300.0f},
      .size = {.width = 50.0f, .height = 50.0f},
      .properties = {.density = 0.1f, .friction = 0.1f, .restitution = 0.8f}};

  dynamicStruct dynamicData2{
      .pos = {.x = 500.0f, .y = 300.0f},
      .size = {.width = 50.0f, .height = 50.0f},
      .properties = {.density = 0.1f, .friction = 0.5f, .restitution = 0.8f}};

  rigidStruct rigidData{.pos = {.x = 0.0f, .y = 500.0f},
                        .size = {.width = 800.0f, .height = 100.0f}};
  rigidStruct rigidData2{.pos = {.x = 300.0f, .y = 500.0f},
                         .size = {.width = 100.0f, .height = 100.0f}};

  world->createDynamicBody(dynamicData);
  world->createDynamicBody(dynamicData2);
  world->createRigidBody(rigidData);

  world->createRigidBody(rigidData2);

  world->sdlSetup();
  while (true)
  {
    world->gameLoop();
  }
  return 0;
}
