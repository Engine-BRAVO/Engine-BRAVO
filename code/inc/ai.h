#ifndef AI_H
#define AI_H

#include "maze.h"
#include <stack>
#include <utility>
#include <Recast.h>
#include <DetourNavMesh.h>
#include <DetourNavMeshQuery.h>

class AI
{
public:
    AI(int startX, int startY, int endX, int endY, const Maze &maze);
    ~AI();
    void update();
    void render() const;

private:
    int startX, startY, endX, endY;
    int posX, posY;
    const Maze &maze;
    std::stack<std::pair<int, int>> path;

    dtNavMesh *navMesh;
    dtNavMeshQuery *navQuery;
    dtQueryFilter filter;

    void buildNavMesh();
    void findPath();
    bool buildRecastNavMesh(dtNavMesh *navMesh, const Maze &maze); // Add this function to build the navmesh
};

#endif // AI_H