#include <SDL2/SDL.h>

#include <iostream>

#include "world.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Floor class definition
class Floor
{
 public:
  Floor(rigidBody body)
      : posX(body.bodyPos.x),
        posY(body.bodyPos.y),
        width(body.size.x),
        height(body.size.y)
  {
  }

  void render(SDL_Renderer* renderer)
  {
    SDL_Rect rect = {static_cast<int>(posX), static_cast<int>(posY),
                     static_cast<int>(width), static_cast<int>(height)};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green color
    SDL_RenderFillRect(renderer, &rect);
  }

 private:
  float posX, posY;
  float width, height;
};

class Square
{
 public:
  Square(dynamicBody body)
      : posX(body.bodyPos.x), posY(body.bodyPos.y), size(body.size.x)
  {
  }

  void move(float deltaX, float deltaY)
  {
    posX = deltaX;
    posY = deltaY;
  }

  void render(SDL_Renderer* renderer)
  {
    SDL_Rect rect = {static_cast<int>(posX), static_cast<int>(posY),
                     static_cast<int>(size), static_cast<int>(size)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red color
    SDL_RenderFillRect(renderer, &rect);
  }

 private:
  float posX, posY;
  float size;
};

int main(int argc, char* argv[])
{
  World* world = new World();
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window =
      SDL_CreateWindow("SDL2 Square with Floor", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  worldPara para;
  para = {.gravity = {.x = 0.0f, .y = 1000.0f}};

  dynamicBody squareShape;
  squareShape = {.bodyPos = {.x = 400, .y = 300},
                 .size = {.x = 50, .y = 50},
                 .density = 0.1f,
                 .friction = 0.5f,
                 .restitution = 0.8f};

  rigidBody staticBody;
  staticBody = {.bodyPos = {.x = 0.0f, .y = 500.f},
                .size = {.x = 800, .y = 200}};

  Square square(squareShape);  // Center the square
  Floor floor(staticBody);     // Create the floor

  bool running = true;
  SDL_Event event;

  world->createWorld(para);
  world->createGroundBody(staticBody);
  world->createDynamicBody(squareShape);

  float timeStep = 1.0f / 60.0f;
  int subStepCount = 4;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
    }
    b2World_Step(world->getWorldId(), timeStep, subStepCount);
    b2Vec2 position = b2Body_GetPosition(world->getBodyId());
    b2Rot rotation = b2Body_GetRotation(world->getBodyId());
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y,
           b2Rot_GetAngle(rotation));

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);

    // Render the floor
    floor.render(renderer);

    // Render the square
    square.render(renderer);

    // Move the square based on physics calculations
    square.move(position.x,
                position.y);  // Adjust these values for different movement

    // Present the renderer
    SDL_RenderPresent(renderer);
    SDL_Delay(16);  // ~60 FPS
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
