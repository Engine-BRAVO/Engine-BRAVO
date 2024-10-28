#include "world.h"

#include <iostream>

#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"

World::World(worldStruct world) : worldData(world)
{
  worldDef = b2DefaultWorldDef();
  worldDef.gravity =
      (b2Vec2){world.gravityX, world.gravityY};  // Configure gravity
  worldId = b2CreateWorld(&worldDef);
}

World::~World() { b2DestroyWorld(worldId); }

void World::sdlSetup()
{
  SDL_Init(SDL_INIT_VIDEO);
  window =
      SDL_CreateWindow("SDL2 Square with Floor", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

float World::pixelToMeter(const float Value)
{
  return (Value * (1.0f / MetersPerPixelFactor));
}

// b2Vec2 World::pixelToMeter(const b2Vec2& rVector)
// {
//   return b2Vec2(pixelToMeter(rVector.x), pixelToMeter(rVector.y));
// }

float World::meterToPixel(const float Value)
{
  return (Value * MetersPerPixelFactor);
}

// b2Vec2 World::meterToPixel(const b2Vec2& rVector)
// {
//   return b2Vec2(meterToPixel(rVector.x), meterToPixel(rVector.y));
// }

void World::sdlRender()
{
  float timeStep = 1.0f / 60.0f;
  int subStepCount = 20;
  b2World_Step(worldId, timeStep, subStepCount);
  // Clear the renderer
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
  SDL_RenderClear(renderer);

  for (auto iterator : rigidBodies)
  {
    rigidSize tempSize = iterator->getRigidSize();
    b2Vec2 tempPos = iterator->getRigidPos();
    SDL_Rect rect = {static_cast<int>(tempPos.x - tempSize.width),
                     static_cast<int>(tempPos.y - tempSize.height),
                     static_cast<int>(tempSize.width * 2),
                     static_cast<int>(tempSize.height * 2)};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color
    SDL_RenderFillRect(renderer, &rect);

    // std::cout << "left side of green: " << tempPos.x << std::endl;
    //  printf("%4.2f %4.2f\n", tempPos.x, tempPos.y);
  }

  for (auto iterator : dynamicBodies)
  {
    iterator->setPosition();
    dynamicSize tempSize = iterator->getSize();
    b2Vec2 tempPos = iterator->getPosition();

    SDL_Rect rect = {static_cast<int>(tempPos.x - tempSize.width),
                     static_cast<int>(tempPos.y - tempSize.height),
                     static_cast<int>(tempSize.width * 2),
                     static_cast<int>(tempSize.height * 2)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &rect);
    //  std::cout << "right side of red: " << tempPos.x + tempSize.width
    //          << std::endl;
    // printf("%4.2f %4.2f\n", tempPos.x, tempPos.y);
  }

  // Present the renderer
  SDL_RenderPresent(renderer);
  // SDL_Delay(16);  // ~60 FPS

  //     b2Rot_GetAngle(rotation));
}

void World::gameLoop()
{
  bool running = true;
  SDL_Event event;

  float timeStep = 1.0f / 60.0f;
  int subStepCount = 4;
  b2BodyId bodyid;
  b2Vec2 groundpos;
  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
          /* Look for a keypress */
        case SDL_KEYDOWN:
          /* Check the SDLKey values and move change the coords */
          switch (event.key.keysym.sym)
          {
            case SDLK_LEFT:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {-2000.0f, 0.0f},
                  true);

              break;
            case SDLK_RIGHT:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {2000.0f, 0.0f},
                  true);

              break;
            case SDLK_UP:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {0.0, -2000.0f},
                  true);

              break;
            case SDLK_DOWN:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {0.0, 2000.0f},
                  true);

              break;
            case SDLK_q:
              running = false;
              break;
            case SDLK_1:
              currentCube = 0;
              break;
            case SDLK_2:
              currentCube = 1;
              break;
            case SDLK_3:
              // currentCube = 1;
              bodyid = rigidBodies.at(0)->getGroundid();
              groundpos = b2Body_GetPosition(bodyid);
              b2Body_SetTransform(bodyid, {groundpos.x - 5, groundpos.y},
                                  {cos(0.0f), 0.0f});

              break;
            case SDLK_4:
              // currentCube = 1;
              bodyid = rigidBodies.at(0)->getGroundid();
              groundpos = b2Body_GetPosition(bodyid);
              b2Body_SetTransform(bodyid, {groundpos.x + 5, groundpos.y},
                                  {cos(0.0f), 0.0f});

              // currentCube = 1;
              break;
            case SDLK_5:
              bodyid = dynamicBodies.at(0)->getBodyId();
              b2Body_SetLinearVelocity(bodyid, {50.0f, 0.0f});

              break;
            default:
              break;
          }
      }
    }

    this->sdlRender();
    contactListener.setContactList(worldId);
    contactListener.getContact();
    contactListener.getBodyContact(dynamicBodies[0]->getBodyId());
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void World::setFrictionJoins() {}

void World::createWorld(worldStruct world) {}

void World::createRigidBody(rigidStruct rigid)
{
  rigidBodies.push_back(new RigidBody(rigid, worldId));
}

void World::createDynamicBody(dynamicStruct dynamic)
{
  dynamicBodies.push_back(new DynamicBody(dynamic, worldId));
}

b2WorldId World::getWorldId() { return worldId; }
