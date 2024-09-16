#pragma once

#include "ParticleSystem.h"
#include "SDL_render.h"
#include <SDL2/SDL.h>

class Application 
{
public:
	Application();
	~Application();

	void update();
	void draw();

	SDL_Surface *load_surface(char const *path);

private:
	ParticleSystem *particlesystem;

	SDL_Surface *m_image;
	SDL_Rect m_image_position;

	SDL_Renderer *renderer;

	SDL_Window *m_window;
	SDL_Surface *m_window_surface;
	SDL_Event m_window_event;
};
