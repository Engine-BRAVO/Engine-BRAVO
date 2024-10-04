#include "ai.h"

#include <cmath>
#include <algorithm>
#include <iostream>

ai::ai(maze &aMaze) : mMaze(aMaze)
{
    mCurrentLocation = {-1, -1};
    mNextLocation = mCurrentLocation;
    mEndLocation = mCurrentLocation;
}

ai::~ai()
{
    while (!mOpenSet.empty())
    {
        delete mOpenSet.top();
        mOpenSet.pop();
    }
    for (const auto &node : mClosedSet)
    {
        delete node.second;
    }
    for (const auto &node : mPath)
    {
        delete node;
    }
}

void ai::setLocation(Location aLocation)
{
    mCurrentLocation = aLocation;
    mNextLocation = aLocation;
    mEndLocation = aLocation;
}

void ai::setEndLocation(Location aLocation)
{
    mEndLocation = aLocation;
}

Location ai::getLocation() const
{
    return mCurrentLocation;
}

void ai::updateLocation()
{
    if (mCurrentLocation.x == mEndLocation.x && mCurrentLocation.y == mEndLocation.y) // if at end do nothing
    {
        return;
    }
    // Check if current location is already at the next location
    if (mCurrentLocation.x == mNextLocation.x && mCurrentLocation.y == mNextLocation.y)
    {
        // If path is not empty, set the next location to the next node in the path
        if (!mPath.empty())
        {
            mNextLocation = mPath.back()->mLocation;
            mPath.pop_back();
        }
    }

    // Move towards the next location by one step
    if (mCurrentLocation.x != mNextLocation.x && mCurrentLocation.y != mNextLocation.y)
    {
        // Move diagonally
        if (mCurrentLocation.x < mNextLocation.x)
        {
            mCurrentLocation.x++;
        }
        else
        {
            mCurrentLocation.x--;
        }
        if (mCurrentLocation.y < mNextLocation.y)
        {
            mCurrentLocation.y++;
        }
        else
        {
            mCurrentLocation.y--;
        }
    }
    else if (mCurrentLocation.x != mNextLocation.x)
    {
        // Move horizontally
        if (mCurrentLocation.x < mNextLocation.x)
        {
            mCurrentLocation.x++;
        }
        else
        {
            mCurrentLocation.x--;
        }
    }
    else if (mCurrentLocation.y != mNextLocation.y)
    {
        // Move vertically
        if (mCurrentLocation.y < mNextLocation.y)
        {
            mCurrentLocation.y++;
        }
        else
        {
            mCurrentLocation.y--;
        }
    }
}

void ai::findPath()
{
    // Cleanup before starting the pathfinding
    cleanup();

    // Lambda function to calculate the heuristic (Manhattan distance)
    auto heuristic = [](Location a, Location b)
    {
        return std::abs(a.x - b.x) * 10 + std::abs(a.y - b.y) * 10;
    };

    // Initialize the start node
    Node *startNode = new Node{mCurrentLocation, 0, heuristic(mCurrentLocation, mEndLocation), 0, nullptr};
    startNode->fCost = startNode->gCost + startNode->hCost;

    // Add the start node to the open set
    mOpenSet.push(startNode);

    while (!mOpenSet.empty())
    {
        // Get the node with the lowest fCost
        Node *currentNode = mOpenSet.top();
        mOpenSet.pop();

        // If we reached the end location, reconstruct the path
        if (currentNode->mLocation.x == mEndLocation.x && currentNode->mLocation.y == mEndLocation.y)
        {
            while (currentNode != nullptr)
            {
                mPath.push_back(currentNode);
                currentNode = currentNode->mParent;
            }
            mPath.pop_back(); // Remove the start node

            for (auto node : mPath)
            {
                std::cout << node->mLocation.x << " " << node->mLocation.y << std::endl;
            }

            return;
        }

        // Add the current node to the closed set
        mClosedSet[currentNode->mLocation.x * 1000 + currentNode->mLocation.y] = currentNode;

        // Check each neighbor of the current node
        std::array<Location, 8> neighbors = {
            Location{currentNode->mLocation.x + 1, currentNode->mLocation.y},      // right
            Location{currentNode->mLocation.x - 1, currentNode->mLocation.y},      // left
            Location{currentNode->mLocation.x, currentNode->mLocation.y + 1},      // down
            Location{currentNode->mLocation.x, currentNode->mLocation.y - 1},      // up
            Location{currentNode->mLocation.x + 1, currentNode->mLocation.y + 1},  // down-right
            Location{currentNode->mLocation.x - 1, currentNode->mLocation.y + 1},  // down-left
            Location{currentNode->mLocation.x + 1, currentNode->mLocation.y - 1},  // up-right
            Location{currentNode->mLocation.x - 1, currentNode->mLocation.y - 1}}; // up-left

        for (const auto &neighborLoc : neighbors)
        {
            // Skip if the neighbor is in the closed set
            if (mClosedSet.find(neighborLoc.x * 1000 + neighborLoc.y) != mClosedSet.end())
            {
                continue;
            }

            // Skip if the neighbor is a wall
            if (mMaze.isWall(neighborLoc.x, neighborLoc.y))
            {
                continue;
            }

            // Calculate the costs for the neighbor
            int gCost = currentNode->gCost + ((neighborLoc.x != currentNode->mLocation.x && neighborLoc.y != currentNode->mLocation.y) ? 14 : 10);
            int hCost = heuristic(neighborLoc, mEndLocation);
            int fCost = gCost + hCost;

            // Check if the neighbor is in the open set
            bool inOpenSet = false;
            Node *neighborNode = nullptr;
            std::priority_queue<Node *, std::vector<Node *>, CompareNode> tempQueue = mOpenSet;
            while (!tempQueue.empty())
            {
                Node *node = tempQueue.top();
                tempQueue.pop();
                if (node->mLocation.x == neighborLoc.x && node->mLocation.y == neighborLoc.y)
                {
                    inOpenSet = true;
                    neighborNode = node;
                    break;
                }
            }

            // If the neighbor is not in the open set or the new path is shorter
            if (!inOpenSet || gCost < neighborNode->gCost)
            {
                if (!inOpenSet)
                {
                    neighborNode = new Node{neighborLoc, gCost, hCost, fCost, currentNode};
                    mOpenSet.push(neighborNode);
                }
                else
                {
                    neighborNode->gCost = gCost;
                    neighborNode->hCost = hCost;
                    neighborNode->fCost = fCost;
                    neighborNode->mParent = currentNode;
                }
            }
        }
    }
}

const std::vector<Node *> &ai::getPath() const
{
    return mPath;
}

void ai::cleanup()
{
    // Clear the open set
    while (!mOpenSet.empty())
    {
        delete mOpenSet.top();
        mOpenSet.pop();
    }

    // Clear the closed set
    for (const auto &node : mClosedSet)
    {
        delete node.second;
    }
    mClosedSet.clear();

    // Clear the path
    for (const auto &node : mPath)
    {
        delete node;
    }
    mPath.clear();
}