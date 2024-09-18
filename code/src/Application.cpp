#include <iostream>

#include "Application.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"

Application::Application() {

    // m_window = SDL_CreateWindow("sdl2 window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640, 480, 0, &m_window, &renderer);
    //	m_window_surface = SDL_GetWindowSurface(m_window);
    SDL_RenderSetScale(renderer, 4, 4);

    emitter particleEmitter;

    particleEmitter.x = 30;
    particleEmitter.y = 100;
    particleEmitter.amount = 500;
    particleEmitter.lifeSpan = 50;
    particleEmitter.lifeSpanVar = 10;
    color startColor = {255, 255, 255};
    color endColor = {255, 0, 0};
    particleEmitter.startColor = startColor;
    particleEmitter.endColor = endColor;

    ParticleSystem* part = new ParticleSystem(particleEmitter);
    particlesystem = part;
}

Application::~Application() {
    SDL_FreeSurface(m_window_surface);
    SDL_DestroyWindow(m_window);
}

void Application::update() {

    bool keep_window_open = true;
    while (keep_window_open) {
        while (SDL_PollEvent(&m_window_event) > 0) {
            switch (m_window_event.type) {
            case SDL_QUIT:
                keep_window_open = false;
                break;
            }
        }
        draw();
    }
}

void Application::draw() {
    particlesystem->updateParticles();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    std::vector<Particle> particleList = particlesystem->getParticles();
    for (std::vector<Particle>::iterator it = particleList.begin(); it != particleList.end(); ++it) {
        color tempColor = it->getColor();
        SDL_SetRenderDrawColor(renderer, tempColor.r, tempColor.g, tempColor.b, 0);

        SDL_RenderDrawPoint(renderer, it->_pos.y, it->_pos.x);
        SDL_RenderPresent(renderer);
        SDL_UpdateWindowSurface(m_window);
    }
    SDL_Delay(16);
}

SDL_Surface* Application::load_surface(char const* path) {
    SDL_Surface* image_surface = SDL_LoadBMP(path);

    if (!image_surface) {
        return 0;
    }
    return image_surface;
}
