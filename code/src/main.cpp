#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "ParticleSystem.h"
#include "SDL_video.h"
#include "Application.h"

int main(int argc, char* args[]) 
{
	Application app;
	app.update();
	app.draw();
}
