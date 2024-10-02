#ifndef MAZENODE_H
#define MAZENODE_H

#include <memory>
#include <vector>

#include "mazeEdge.h"

class mazeNode
{
public:
    mazeNode(bool aIsFloor);
    virtual ~mazeNode() = default;

    void addEdge(std::unique_ptr<mazeEdge> aEdge);
    bool isFloor() const;
    void setFloor(bool aIsFloor);

private:
    bool mIsFloor;
    std::vector<std::unique_ptr<mazeEdge>> mEdges;
};

#endif // MAZENODE_H