#include "game.h"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <vector>
#include <algorithm>

#include "ai.h"

game::game()
{
    mMaze = std::make_unique<maze>(MAZE_SIZE, MAZE_SIZE);
    mAI = std::make_unique<ai>(*mMaze);
    mStartSet = false;
}

void game::init()
{
    std::srand(std::time(nullptr));
    generateMaze();
}

void game::generateMaze()
{
    // Initialize the maze with walls
    for (int i = 0; i < MAZE_SIZE; ++i)
    {
        for (int j = 0; j < MAZE_SIZE; ++j)
        {
            mMaze->setWall(i, j, true);
        }
    }

    std::vector<std::vector<bool>> visited(MAZE_SIZE, std::vector<bool>(MAZE_SIZE, false));
    std::stack<std::pair<int, int>> stack;

    // Start from the top-left corner
    int startX = 0;
    int startY = 0;
    stack.push({startX, startY});
    visited[startX][startY] = true;
    mMaze->setWall(startX, startY, false); // Set the starting cell as floor

    std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    while (!stack.empty())
    {
        auto [x, y] = stack.top();
        stack.pop();

        std::vector<std::pair<int, int>> neighbors;
        for (const auto &[dx, dy] : directions)
        {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE && !visited[nx][ny])
            {
                neighbors.push_back({nx, ny});
            }
        }

        if (!neighbors.empty())
        {
            stack.push({x, y});
            auto [nx, ny] = neighbors[std::rand() % neighbors.size()];
            visited[nx][ny] = true;
            mMaze->setWall(nx, ny, false);                     // Set the cell as floor
            mMaze->setWall((x + nx) / 2, (y + ny) / 2, false); // Remove wall between cells
            stack.push({nx, ny});
        }
    }

    // Randomly place additional walls
    int totalCells = MAZE_SIZE * MAZE_SIZE;
    int wallCount = totalCells * 0.4;
    int placedWalls = 0;
    while (placedWalls < wallCount)
    {
        int row = std::rand() % MAZE_SIZE;
        int col = std::rand() % MAZE_SIZE;
        if (mMaze->getMaze()[row][col]->isFloor() && (row != 0 || col != 0))
        {
            mMaze->setWall(row, col, true);
            placedWalls++;
        }
    }
}

void game::run()
{
    init();
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return;
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int screenWidth = DM.w * 0.8;
    int screenHeight = DM.h * 0.8;

    // Calculate the tile size based on the screen dimensions and maze dimensions
    int tileSize = std::min(screenWidth / MAZE_SIZE, screenHeight / MAZE_SIZE);

    // Adjust the window size to match the maze dimensions multiplied by the tile size
    screenWidth = tileSize * MAZE_SIZE;
    screenHeight = tileSize * MAZE_SIZE;

    SDL_Window *window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    bool quit = false;
    SDL_Event e;
    Uint32 lastUpdate = SDL_GetTicks();
    Uint32 lastRender = SDL_GetTicks();
    const Uint32 updateInterval = 1000;      // 1 tick per second
    const Uint32 renderInterval = 1000 / 60; // 60 FPS

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int row = y / tileSize;
                int col = x / tileSize;

                if (!mStartSet)
                {
                    mAI->setLocation({col, row});
                    mStartSet = true;
                }
                else
                {
                    mAI->setEndLocation({col, row});
                    mAI->findPath();
                }
            }
        }

        Uint32 current = SDL_GetTicks();
        if (current - lastUpdate >= updateInterval)
        {
            // Update game logic here (e.g., AI movement)
            mAI->updateLocation();
            lastUpdate = current;
        }

        if (current - lastRender >= renderInterval)
        {
            render(renderer, tileSize);
            lastRender = current;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game::render(SDL_Renderer *renderer, int tileSize)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < MAZE_SIZE; ++i)
    {
        for (int j = 0; j < MAZE_SIZE; ++j)
        {
            if (mMaze->getMaze()[i][j]->isFloor())
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for floor
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray for walls
            }

            SDL_Rect tileRect = {j * tileSize, i * tileSize, tileSize, tileSize};
            SDL_RenderFillRect(renderer, &tileRect);

            // Draw black border
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &tileRect);
        }
    }

    // Render AI path
    const std::vector<Node *> &path = mAI->getPath();
    for (const auto &node : path)
    {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange for path
        SDL_Rect pathRect = {node->mLocation.x * tileSize, node->mLocation.y * tileSize, tileSize, tileSize};
        SDL_RenderFillRect(renderer, &pathRect);
    }

    // Render AI
    Location aiLocation = mAI->getLocation();
    if (!(aiLocation.x == -1))
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for AI
        SDL_Rect aiRect = {aiLocation.x * tileSize, aiLocation.y * tileSize, tileSize, tileSize};
        SDL_RenderFillRect(renderer, &aiRect);
    }
    SDL_RenderPresent(renderer);
}

const maze &game::getMaze() const
{
    return *mMaze;
}