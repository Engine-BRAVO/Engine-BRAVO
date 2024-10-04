#ifndef AI_H
#define AI_H

#include "maze.h"

#include <vector>
#include <unordered_map>
#include <queue>

struct Location
{
    int x;
    int y;
};

struct Node
{
    Location mLocation;
    int gCost;
    int hCost;
    int fCost;
    Node *mParent;
};

struct CompareNode
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        if (lhs->fCost == rhs->fCost)
        {
            return lhs->hCost > rhs->hCost;
        }
        return lhs->fCost > rhs->fCost;
    }
};

class ai
{
public:
    ai(maze &aMaze);
    virtual ~ai();

    void setLocation(Location aLocation);
    void setEndLocation(Location aLocation);
    Location getLocation() const;

    void updateLocation();

private:
    maze &mMaze;
    Location mCurrentLocation;
    Location mEndLocation;
    Location mNextLocation;

public:
    void findPath();
    const std::vector<Node *> &getPath() const;

private:
    void cleanup();

private:
    std::vector<Node *> mPath;
    std::priority_queue<Node *, std::vector<Node *>, CompareNode> mOpenSet;
    std::unordered_map<int, Node *> mClosedSet;
};

#endif // AI_H