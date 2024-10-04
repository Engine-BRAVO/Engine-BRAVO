#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <memory>
#include "mazeNode.h"

class maze
{
public:
    maze(int aRows, int aCols);
    virtual ~maze() = default;

    void setWall(int aRow, int aCol, bool aIsWall);
    const std::vector<std::vector<std::unique_ptr<mazeNode>>> &getMaze() const;

    bool isWall(int aRow, int aCol) const;

private:
    int mRows;
    int mCols;
    std::vector<std::vector<std::unique_ptr<mazeNode>>> mNodes;

    void createEdges();
};

#endif // MAZE_H