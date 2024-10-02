#ifndef GAME_H
#define GAME_H

#include "maze.h"

#include <memory>
#include <SDL2/SDL.h>

#define MAZE_SIZE 40

class game
{
public:
    game();
    virtual ~game() = default;

    void run();

    const maze &getMaze() const;

private:
    std::unique_ptr<maze> mMaze;
    void generateMaze();

    void render(SDL_Renderer *renderer, int tileSize);

    void init();
};

#endif // GAME_H