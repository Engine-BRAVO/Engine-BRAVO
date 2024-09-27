#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "ai.h"
#include <thread>
#include <chrono>
#include <SDL2/SDL.h>

class Game
{
public:
    Game();
    ~Game();
    void run();

private:
    Maze maze;
    AI ai;
    SDL_Window *window;
    SDL_Renderer *renderer;

    void render();
};

#endif // GAME_H