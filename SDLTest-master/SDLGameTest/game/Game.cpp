#include "Game.h"

#include "../dodf/MemoryPool.h"

Game::Game()
{

}

int Game::init()
{
	int error = 0;
	error += initSDL();
	size_t initSize = 1 * 1024 * 1024 * 1024; //1GB
	dodf::MemoryPool::Initialize(initSize);
	//m_transformComponentMgr.allocate(100);
	m_enemyComponentMgr.allocate(100);
	initEnemies();

	return error;
}

void Game::initEnemies()
{
	m_enemyComponentMgr.allocate(100);
	m_enemies = (Entity*)dodf::MemoryPool::Get(sizeof(Entity) * ENEMY_COUNT);
	const size_t NUMBER_COLUMNS = 11;
	const float LEFT_MARGIN = 70.0f;
	const float TOP_MARGIN = 90.0f;
	const float ENEMY_SIZE = EnemyComponentMgr::ENEMY_SIZE;
	const float HORIZONTAL_MARGIN_BETWEEN_ENEMIES = 3.0f;
	const float VERTICAL_MARGIN_BETWEEN_ENEMIES = 3.0f;
	for (size_t i = 0; i < ENEMY_COUNT; ++i) {
		m_enemies[i] = m_entityManager.create();
		glm::vec3 position;
		//position.x = i * EnemyComponentMgr::ENEMY_SIZE;
		size_t horizontalIndex = i % NUMBER_COLUMNS;
		position.x = LEFT_MARGIN + static_cast<float>(horizontalIndex) * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES);
		size_t verticalIndex = i / NUMBER_COLUMNS;
		position.y = TOP_MARGIN + static_cast<float>(verticalIndex) * (ENEMY_SIZE + VERTICAL_MARGIN_BETWEEN_ENEMIES);
		position.z = 0.0f;
		m_enemyComponentMgr.add(m_enemies[i], position);
	}		
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
	
	SDL_Rect texture_rect;
	texture_rect.x = 0;   // the x coordinate
	texture_rect.y = 0;   // the y coordinate
	texture_rect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	texture_rect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture

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

		/*SDL_Rect texture_rect;
		texture_rect.x = 0;   // the x coordinate
		texture_rect.y = 0;   // the y coordinate
		texture_rect.w = 64;  // the width of the texture
		texture_rect.h = 128;  // the height of the texture


		SDL_Rect texture_rect_;
		texture_rect_.x = 64;   // the x coordinate
		texture_rect_.y = 0;   // the y coordinate
		texture_rect_.w = 64;  // the width of the texture
		texture_rect_.h = 64;  // the height of the texture

		SDL_RenderCopy(m_renderer, texture, nullptr, &texture_rect_);*/



		SDL_RenderClear(m_renderer);

		auto positions = m_enemyComponentMgr.getPositions();
		for (size_t i = 0; i < ENEMY_COUNT; ++i) {
			texture_rect.x = positions[i].x;
			SDL_RenderCopy(m_renderer, texture, nullptr, &texture_rect);
		}
		
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
