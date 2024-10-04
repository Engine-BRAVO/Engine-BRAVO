#include "maze.h"
#include "mazeEdge.h"

maze::maze(int aRows, int aCols) : mRows(aRows), mCols(aCols)
{
    mNodes.resize(aRows);
    for (int i = 0; i < aRows; ++i)
    {
        mNodes[i].resize(aCols);
        for (int j = 0; j < aCols; ++j)
        {
            mNodes[i][j] = std::make_unique<mazeNode>(true); // Initialize all nodes as floors
        }
    }
    createEdges();
}

void maze::setWall(int aRow, int aCol, bool aIsWall)
{
    if (aRow >= 0 && aRow < mRows && aCol >= 0 && aCol < mCols)
    {
        mNodes[aRow][aCol].get()->setFloor(!aIsWall);
    }
}

const std::vector<std::vector<std::unique_ptr<mazeNode>>> &maze::getMaze() const
{
    return mNodes;
}

bool maze::isWall(int aRow, int aCol) const
{
    if (aRow < 0 || aRow >= mRows || aCol < 0 || aCol >= mCols)
    {
        return true; // Out of bounds is considered a wall
    }
    return !mNodes[aRow][aCol]->isFloor();
}

void maze::createEdges()
{
    for (int i = 0; i < mRows; ++i)
    {
        for (int j = 0; j < mCols; ++j)
        {
            if (i > 0) // Connect to the node above
            {
                mNodes[i][j]->addEdge(std::move(std::make_unique<mazeEdge>(*mNodes[i - 1][j])));
            }
            if (i < mRows - 1) // Connect to the node below
            {
                mNodes[i][j]->addEdge(std::move(std::make_unique<mazeEdge>(*mNodes[i + 1][j])));
            }
            if (j > 0) // Connect to the node to the left
            {
                mNodes[i][j]->addEdge(std::move(std::make_unique<mazeEdge>(*mNodes[i][j - 1])));
            }
            if (j < mCols - 1) // Connect to the node to the right
            {
                mNodes[i][j]->addEdge(std::move(std::make_unique<mazeEdge>(*mNodes[i][j + 1])));
            }
        }
    }
}