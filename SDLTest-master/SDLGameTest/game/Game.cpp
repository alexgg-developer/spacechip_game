#include "Game.h"

#include "../dodf/MemoryPool.h"

#include "Vec.h"


Game::Game(): m_playerController(m_player)
{

}

Game::~Game()
{
	dodf::MemoryPool::Destroy();

	//TO DO: ADD ALL TEXTURES AND SURFACES
	//SDL_DestroyTexture(texture);
	//SDL_FreeSurface(space_ship_surface);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int Game::init()
{
	int error = 0;
	error += initSDL();
	if (!error) {
		size_t initSize = 1 * 1024 * 1024 * 1024; //1GB
		dodf::MemoryPool::Initialize(initSize);
		m_enemyComponentMgr.allocate(100);
		initEnemies();
		initPlayer();
	}

	return error;
}

void Game::initEnemies()
{
	m_enemyComponentMgr.allocate(ENEMY_COUNT);
	m_enemies = (Entity*)dodf::MemoryPool::Get(sizeof(Entity) * ENEMY_COUNT);
	const size_t NUMBER_ROWS = 5u;
	const size_t NUMBER_COLUMNS = (ENEMY_COUNT / NUMBER_ROWS);
	const size_t LEFT_MARGIN = 70u;
	const size_t TOP_MARGIN = 90u;
	const size_t ENEMY_SIZE = EnemyComponentMgr::ENEMY_SIZE;
	const size_t HORIZONTAL_MARGIN_BETWEEN_ENEMIES = 3u;
	const size_t VERTICAL_MARGIN_BETWEEN_ENEMIES = 3u;
	for (size_t i = 0; i < ENEMY_COUNT; ++i) {
		m_enemies[i] = m_entityManager.create();
		vec3 position;
		//position.x = i * EnemyComponentMgr::ENEMY_SIZE;
		size_t horizontalIndex = i % NUMBER_COLUMNS;
		position.x = static_cast<float>(LEFT_MARGIN + horizontalIndex * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES));
		size_t verticalIndex = i / NUMBER_COLUMNS;
		position.y = static_cast<float>(TOP_MARGIN + verticalIndex * (ENEMY_SIZE + VERTICAL_MARGIN_BETWEEN_ENEMIES));
		position.z = 0.0f;
		m_enemyComponentMgr.add(m_enemies[i], position);
	}		
}

void Game::initPlayer()
{
	vec3 position;
	position.x = 70.0f;
	position.y = static_cast<float>(WINDOW_HEIGHT - 100u);
	m_player.setPosition(position);
	m_playerController.setLimits(30.0f, static_cast<float>(WINDOW_WIDTH) - 30.0f - static_cast<float>(Player::SIZE));
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

	SDL_Surface *space_ship_enemy_surface = IMG_Load("Assets/Enemy/spaceship_enemy_red.png");
	if (!space_ship_surface) {
		std::cout << "IMG_Load Error: " << SDL_GetError() << std::endl;
		//return 0;
	}

	SDL_Texture *texture_enemy = SDL_CreateTextureFromSurface(m_renderer, space_ship_enemy_surface);
	if (!texture) {
		std::cout << "IMG_LoadTexture Error: " << SDL_GetError() << std::endl;
		//return 0;
	}

	//while()
	SDL_Event e;
	
	SDL_Rect texture_rect;
	texture_rect.x = 0;   // the x coordinate
	texture_rect.y = 0;   // the y coordinate
	texture_rect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	texture_rect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture

	m_timer.start();

	while (!m_input.check(Input::KESC)) {
		SDL_PollEvent(&e);
		m_input.read(e);
		m_playerController.update(m_input, m_timer.getDeltaTime());
		
		SDL_RenderClear(m_renderer);

		auto positions = m_enemyComponentMgr.getPositions();
		for (size_t i = 0; i < ENEMY_COUNT; ++i) {
			texture_rect.x = (int)positions[i].x;
			texture_rect.y = (int)positions[i].y;
			SDL_RenderCopy(m_renderer, texture_enemy, nullptr, &texture_rect);
		}

		auto playerPosition = m_player.getPosition();
		texture_rect.x = (int)playerPosition.x;   // the x coordinate
		texture_rect.y = (int)playerPosition.y;   // the y coordinate
		texture_rect.w = (int)Player::SIZE;  // the width of the texture
		texture_rect.h = (int)Player::SIZE;  // the height of the texture
		SDL_RenderCopy(m_renderer, texture, nullptr, &texture_rect);
		
		SDL_RenderPresent(m_renderer);
	}
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
		SDL_CreateWindow("Space invaders!", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
