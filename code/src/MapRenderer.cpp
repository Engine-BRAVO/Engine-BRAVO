#include "MapRenderer.h"
#include <iostream>
#include <unordered_map>

MapRenderer::MapRenderer() : window(nullptr), renderer(nullptr), camera(800, 600) {}

MapRenderer::~MapRenderer()
{
    cleanup();
}

bool MapRenderer::initialize()
{
    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("TMX Map Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

SDL_Texture* MapRenderer::loadTexture(const std::string& path)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture)
    {
        std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
    }
    return texture;
}

void MapRenderer::renderMap(const tmx::Map& map)
{
    std::cout << "Rendering map..." << std::endl;

    // Load textures for each tileset
    std::unordered_map<int, SDL_Texture*> tilesetTextures;
    for (const auto& tileset : map.getTilesets())
    {
        SDL_Texture* texture = loadTexture(tileset.getImagePath());
        if (!texture)
        {
            std::cerr << "Failed to load texture: " << tileset.getImagePath() << std::endl;
            return;
        }
        tilesetTextures[tileset.getFirstGID()] = texture;
    }

    const auto& layers = map.getLayers();
    for (const auto& layer : layers)
    {
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            std::cout << "Rendering tile layer: " << layer->getName() << std::endl;
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            const auto& mapSize = map.getTileCount();
            const auto& tileSize = map.getTileSize();
            
            float zoomFactor = 0.25f;

            for (unsigned y = 0; y < mapSize.y; ++y)
            {
                for (unsigned x = 0; x < mapSize.x; ++x)
                {
                    const auto& tile = tiles[y * mapSize.x + x];
                    if (tile.ID == 0) continue; // Skip empty tiles

                    // Find the correct tileset for the tile
                    SDL_Texture* texture = nullptr;
                    int tilesetColumns = 0;
                    for (const auto& tileset : map.getTilesets())
                    {
                        if (tile.ID >= tileset.getFirstGID() && tile.ID < tileset.getFirstGID() + tileset.getTileCount())
                        {
                            texture = tilesetTextures[tileset.getFirstGID()];
                            tilesetColumns = tileset.getImageSize().x / tileSize.x;
                            break;
                        }
                    }

                    if (!texture || tilesetColumns == 0)
                    {
                        std::cerr << "Error: Could not find texture or tilesetColumns is zero for tile ID " << tile.ID << std::endl;
                        continue;
                    }

                    SDL_Rect srcRect;
                    srcRect.x = ((tile.ID - 1) % tilesetColumns) * tileSize.x;
                    srcRect.y = ((tile.ID - 1) / tilesetColumns) * tileSize.y;
                    srcRect.w = tileSize.x;
                    srcRect.h = tileSize.y;

                    SDL_Rect dstRect;
                    dstRect.x = static_cast<int>((x * tileSize.x - camera.getX()) * camera.getZoom());
                    dstRect.y = static_cast<int>((y * tileSize.y - camera.getY()) * camera.getZoom());
                    dstRect.w = static_cast<int>(tileSize.x * camera.getZoom());
                    dstRect.h = static_cast<int>(tileSize.y * camera.getZoom());

                    std::cout << "Rendering tile at (" << x << ", " << y << ") with ID " << tile.ID << " on layer " << layer->getName() << std::endl;
                    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
    std::cout << "Map rendering completed." << std::endl;
}

void MapRenderer::cleanup()
{
    std::cout << "Cleaning up..." << std::endl;
    for (auto texture : tilesetTextures)
    {
        SDL_DestroyTexture(texture);
    }
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
    std::cout << "Cleanup completed." << std::endl;
}

void MapRenderer::run(const tmx::Map& map)
{
    if (!initialize())
    {
        std::cerr << "Failed to initialize!" << std::endl;
        return;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                        camera.move(0, -10);
                        break;
                    case SDLK_DOWN:
                        camera.move(0, 10);
                        break;
                    case SDLK_LEFT:
                        camera.move(-10, 0);
                        break;
                    case SDLK_RIGHT:
                        camera.move(10, 0);
                        break;
                    case SDLK_PLUS:
                    case SDLK_EQUALS:
                        camera.setZoom(camera.getZoom() * 1.1f); // Zoom in
                        break;
                    case SDLK_MINUS:
                        camera.setZoom(camera.getZoom() / 1.1f); // Zoom out
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        renderMap(map);
        SDL_Delay(16);
    }

    cleanup();
}