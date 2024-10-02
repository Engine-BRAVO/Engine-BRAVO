#ifndef AI_H
#define AI_H

#include "maze.h"

#include <vector>
#include <unordered_map>

class ai
{
public:
    ai(maze &aMaze);

private:
    maze &mMaze;

    struct Node
    {
        int x;
        int y;
        int gCost;
        int hCost;
        int fCost;
        Node *parent;
    };

    std::vector<std::pair<int, int>> mPath;
    std::unordered_map<int, Node *> mOpenSet;
    std::unordered_map<int, Node *> mClosedSet;
};

#endif // AI_H