#ifndef MAZEEDGE_H
#define MAZEEDGE_H

#include <memory>
#include <vector>

class mazeNode;

class mazeEdge
{
public:
    mazeEdge(mazeNode &aDestination, int aWeight = 1);
    virtual ~mazeEdge() = default;

    const mazeNode &getDestination() const;
    int getWeight() const;

private:
    mazeNode &mDestination;
    int mWeight;
};

#endif // MAZEEDGE_H