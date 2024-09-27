#include "game.h"
#include <iostream>

Game::Game() : maze(10, 10), ai(0, 0, 9, 9, maze)
{
    maze.setWall(1, 0);
    maze.setWall(1, 1);
    maze.setWall(1, 2);
    maze.setWall(1, 3);
    maze.setWall(1, 4);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

Game::~Game()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run()
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        ai.update();
        render();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Render the maze
    const int cellSize = 80;
    for (int y = 0; y < maze.getHeight(); ++y)
    {
        for (int x = 0; x < maze.getWidth(); ++x)
        {
            if (!maze.isWalkable(x, y))
            {
                SDL_Rect wallRect = {x * cellSize, y * cellSize, cellSize, cellSize};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &wallRect);
            }
        }
    }

    // Render the AI
    SDL_Rect aiRect = {ai.getPosX() * cellSize, ai.getPosY() * cellSize, cellSize, cellSize};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &aiRect);

    SDL_RenderPresent(renderer);
}