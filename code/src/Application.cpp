#include <iostream>

#include "Application.h"
#include "ParticleSystem.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"

Application::Application()
{

	//m_window = SDL_CreateWindow("sdl2 window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 680, 480, 0);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(640*4, 480*4, 0, &m_window, &renderer);
	//	m_window_surface = SDL_GetWindowSurface(m_window);	
	SDL_RenderSetScale(renderer, 4, 4);
	
}

Application::~Application()
{
	SDL_FreeSurface(m_window_surface);
	SDL_DestroyWindow(m_window);
}

void Application::update()
{

	
	bool keep_window_open = true;
	while(keep_window_open)
	{
		while(SDL_PollEvent(&m_window_event) > 0)
		{
			switch(m_window_event.type)
			{
				case SDL_QUIT:
					keep_window_open = false;
					break;
			}

		}
		draw();
	}

}

void Application::draw()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for(int i = 0; i < 500; i++) 
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(renderer,500, i);


		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(m_window);

	}

}

SDL_Surface* Application::load_surface(char const *path)
{
	SDL_Surface *image_surface = SDL_LoadBMP(path);

	if(!image_surface) 
	{
		return 0;
	}
	return image_surface;
}
