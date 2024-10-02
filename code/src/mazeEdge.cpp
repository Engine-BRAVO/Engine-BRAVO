#include "mazeEdge.h"

mazeEdge::mazeEdge(mazeNode &aDestination, int aWeight)
    : mDestination(aDestination), mWeight(aWeight) {}

const mazeNode &mazeEdge::getDestination() const
{
    return mDestination;
}

int mazeEdge::getWeight() const
{
    return mWeight;
}