#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#include <iostream>

class Game
{
public: 
	Game();
	void draw();
	int init();
	void run();

private:
	SDL_Renderer * m_renderer;
	SDL_Window * m_window;

	int initSDL();
};
