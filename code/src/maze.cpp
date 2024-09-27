#include "maze.h"

Maze::Maze(int width, int height) : width(width), height(height)
{
    grid = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));
}

void Maze::setWall(int x, int y)
{
    grid[y][x] = 1;
}

bool Maze::isWalkable(int x, int y) const
{
    return grid[y][x] == 0;
}

int Maze::getWidth() const
{
    return width;
}

int Maze::getHeight() const
{
    return height;
}

const std::vector<std::vector<int>> &Maze::getGrid() const
{
    return grid;
}