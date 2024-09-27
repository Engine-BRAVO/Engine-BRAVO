#include <SDL2/SDL.h>

#include <iostream>

#include "world.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class Square
{
 public:
  Square(float x, float y, float size) : posX(x), posY(y), size(size) {}

  void move(float deltaX, float deltaY)
  {
    posX += deltaX;
    posY += deltaY;
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
      SDL_CreateWindow("SDL2 Square", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  Square square(WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT / 2 - 25,
                50);  // Center the square

  bool running = true;
  SDL_Event event;

  world->createWorld();
  world->createGroundBody();
  world->createDynamicBody();

  float timeStep = 1.0f / 60.0f;
  int subStepCount = 4;

  for (int i = 0; i < 90; ++i)
  {
    b2World_Step(world->getWorldId(), timeStep, subStepCount);
    b2Vec2 position = b2Body_GetPosition(world->getBodyId());
    b2Rot rotation = b2Body_GetRotation(world->getBodyId());
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y,
           b2Rot_GetAngle(rotation));
    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);

    // Render the square
    square.render(renderer);

    // Move the square (example: move it right and down)
    square.move(position.x,
                position.y);  // Adjust these values for different movement

    // Present the renderer
    SDL_RenderPresent(renderer);

    SDL_Delay(16);  // ~60 FPS
  }

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }
    }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

