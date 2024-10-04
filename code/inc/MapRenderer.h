#pragma once

#include <tmxlite/Map.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <tmxlite/TileLayer.hpp> 
#include <vector>
#include <string>
#include "Camera.h"

class MapRenderer
{
public:
    MapRenderer();
    ~MapRenderer();
    bool initialize();
    void renderMap(const tmx::Map& map);
    void cleanup();
    void run(const tmx::Map& map);
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> tilesetTextures;

    SDL_Texture* loadTexture(const std::string& path);

    Camera camera;
};