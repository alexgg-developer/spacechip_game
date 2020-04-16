#include "Game.h"

Game::Game()
{

}

int Game::init()
{
	int error = 0;
	error += initSDL();

	return error;
}

void Game::run()
{
	SDL_Surface *space_ship_surface = IMG_Load("Assets/Player/spaceship.png");
	if (!space_ship_surface) {
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		//return 0;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, space_ship_surface);
	if (!texture) {
		std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
		//return 0;
	}

	//while()
	SDL_Event e;
	bool running = true;
	while (running) {
		SDL_PollEvent(&e);
		switch (e.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_KEYDOWN:
			break;
		case SDL_USEREVENT:
			break;
		default:
			break;
		}

		SDL_Rect texture_rect;
		texture_rect.x = 0;   // the x coordinate
		texture_rect.y = 0;   // the y coordinate
		texture_rect.w = 64;  // the width of the texture
		texture_rect.h = 128;  // the height of the texture


		SDL_Rect texture_rect_;
		texture_rect_.x = 64;   // the x coordinate
		texture_rect_.y = 0;   // the y coordinate
		texture_rect_.w = 64;  // the width of the texture
		texture_rect_.h = 64;  // the height of the texture

		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, texture, nullptr, &texture_rect);
		SDL_RenderCopy(m_renderer, texture, nullptr, &texture_rect_);
		SDL_RenderPresent(m_renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(space_ship_surface);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Game::draw()
{
}


int Game::initSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	m_window =
		SDL_CreateWindow("Hello World!", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	if (m_window == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	m_renderer = SDL_CreateRenderer(
		m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (m_renderer == nullptr) {
		SDL_DestroyWindow(m_window);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	return 0;
}
