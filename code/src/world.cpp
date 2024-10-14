#include "world.h"

#include "box2d/box2d.h"
#include "box2d/id.h"
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

void World::sdlRender()
{
  float timeStep = 1.0f / 60.0f;
  int subStepCount = 4;
  b2World_Step(worldId, timeStep, subStepCount);
  // Clear the renderer
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
  SDL_RenderClear(renderer);

  for (auto iterator : rigidBodies)
  {
    rigidSize tempSize = iterator->getRigidSize();
    rigidPos tempPos = iterator->getRigidPos();
    SDL_Rect rect = {static_cast<int>(tempPos.x), static_cast<int>(tempPos.y),
                     static_cast<int>(tempSize.width),
                     static_cast<int>(tempSize.height)};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color
    SDL_RenderFillRect(renderer, &rect);
  }

  for (auto iterator : dynamicBodies)
  {
    iterator->setPosition();
    dynamicSize tempSize = iterator->getSize();
    dynamicPos tempPos = iterator->getPosition();

    SDL_Rect rect = {static_cast<int>(tempPos.x), static_cast<int>(tempPos.y),
                     static_cast<int>(tempSize.width),
                     static_cast<int>(tempSize.height)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &rect);
    printf("%4.2f %4.2f\n", tempPos.x, tempPos.y);
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
                  dynamicBodies[currentCube]->getBodyId(), {-500.0f, 0.0f},
                  true);

              break;
            case SDLK_RIGHT:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {500.0f, 0.0f},
                  true);

              break;
            case SDLK_UP:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {0.0, -500.0f},
                  true);

              break;
            case SDLK_DOWN:
              b2Body_ApplyLinearImpulseToCenter(
                  dynamicBodies[currentCube]->getBodyId(), {0.0, 500.0f}, true);

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
            default:
              break;
          }
      }
    }

    this->sdlRender();
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
