#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Dot.h"
#include "Particle.h"

int initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Sprite Sheet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}


void deInitSDL(SDL_Window*& window, SDL_Renderer*& renderer) 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) 
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    initSDL(window, renderer);

    bool quit = false;
    SDL_Event event;
}

