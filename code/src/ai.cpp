// #include "ai.h"
// #include <iostream>
// #include <queue>

// AI::AI(int startX, int startY, int endX, int endY, const Maze &maze)
//     : startX(startX), startY(startY), endX(endX), endY(endY), maze(maze), navMesh(nullptr), navQuery(nullptr)
// {
//     posX = startX;
//     posY = startY;
//     buildNavMesh();
//     findPath();
// }

// AI::~AI()
// {
//     if (navMesh)
//     {
//         dtFreeNavMesh(navMesh);
//     }
//     if (navQuery)
//     {
//         dtFreeNavMeshQuery(navQuery);
//     }
// }

// void AI::buildNavMesh()
// {
//     // Build the navigation mesh using Recast

//     // Initialize navMesh and navQuery
//     navMesh = dtAllocNavMesh();
//     navQuery = dtAllocNavMeshQuery();

//     if (!navMesh || !navQuery)
//     {
//         std::cerr << "Failed to allocate navMesh or navQuery\n";
//         return;
//     }

//     // Load or build your navmesh here
//     // For simplicity, let's assume you have a function to build the navmesh
//     if (!buildRecastNavMesh(navMesh, maze))
//     {
//         std::cerr << "Failed to build navMesh\n";
//         return;
//     }

//     // Initialize the query filter
//     filter.setIncludeFlags(0xFFFF);
//     filter.setExcludeFlags(0);

//     if (dtStatusFailed(navQuery->init(navMesh, 2048)))
//     {
//         std::cerr << "Failed to initialize navQuery\n";
//         return;
//     }
// }

// void AI::findPath()
// {
//     // Use Detour to find a path on the navmesh
//     float startPos[3] = {static_cast<float>(startX), 0, static_cast<float>(startY)};
//     float endPos[3] = {static_cast<float>(endX), 0, static_cast<float>(endY)};

//     dtPolyRef startRef, endRef;
//     float nearestStart[3], nearestEnd[3];

//     if (dtStatusFailed(navQuery->findNearestPoly(startPos, &filter, &startRef, nearestStart)))
//     {
//         std::cerr << "Failed to find nearest poly for start position\n";
//         return;
//     }

//     if (dtStatusFailed(navQuery->findNearestPoly(endPos, &filter, &endRef, nearestEnd)))
//     {
//         std::cerr << "Failed to find nearest poly for end position\n";
//         return;
//     }

//     dtPolyRef pathPolys[256];
//     int pathCount;
//     if (dtStatusFailed(navQuery->findPath(startRef, endRef, nearestStart, nearestEnd, &filter, pathPolys, &pathCount, 256)))
//     {
//         std::cerr << "Failed to find path\n";
//         return;
//     }

//     // Simplify the path
//     float straightPath[256 * 3];
//     unsigned char straightPathFlags[256];
//     dtPolyRef straightPathPolys[256];
//     int straightPathCount;
//     if (dtStatusFailed(navQuery->findStraightPath(nearestStart, nearestEnd, pathPolys, pathCount, straightPath, straightPathFlags, straightPathPolys, &straightPathCount, 256)))
//     {
//         std::cerr << "Failed to find straight path\n";
//         return;
//     }

//     // Convert the path to a stack of coordinates
//     for (int i = straightPathCount - 1; i >= 0; --i)
//     {
//         path.push({static_cast<int>(straightPath[i * 3]), static_cast<int>(straightPath[i * 3 + 2])});
//     }
// }

// void AI::update()
// {
//     if (!path.empty())
//     {
//         posX = path.top().first;
//         posY = path.top().second;
//         path.pop();
//     }
// }

// void AI::render() const
// {
//     std::cout << "AI Position: (" << posX << ", " << posY << ")\n";
// }