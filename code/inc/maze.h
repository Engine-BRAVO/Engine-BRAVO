#ifndef MAZE_H
#define MAZE_H

#include <vector>

class Maze
{
public:
    Maze(int width, int height);
    void setWall(int x, int y);
    bool isWalkable(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    const std::vector<std::vector<int>> &getGrid() const;

private:
    int width, height;
    std::vector<std::vector<int>> grid;
};

#endif // MAZE_H