#include "mazeNode.h"

mazeNode::mazeNode(bool aIsFloor) : mIsFloor(aIsFloor) {}

void mazeNode::addEdge(std::unique_ptr<mazeEdge> aEdge)
{
    mEdges.push_back(std::move(aEdge));
}

bool mazeNode::isFloor() const
{
    return mIsFloor;
}

void mazeNode::setFloor(bool aIsFloor)
{
    mIsFloor = aIsFloor;
}