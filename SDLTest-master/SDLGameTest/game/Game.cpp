#include "Game.h"

#include "../dodf/MemoryPool.h"

#include "Vec.h"


Game::Game(): m_playerController(m_player)
{

}

Game::~Game()
{
	m_textureMgr.clean();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	dodf::MemoryPool::Destroy();
}

int Game::init()
{
	int error = 0;
	error += initSDL();
	if (error) return error;
	size_t initSize = 1 * 1024 * 1024 * 1024; //1GB
	dodf::MemoryPool::Initialize(initSize);
	m_enemyComponentMgr.allocate(100u);
	size_t projectileMax = 30u;
	m_projectileComponentMgr.allocate(projectileMax);
	m_projectiles.reserve(projectileMax);
	initEnemies();
	initPlayer();	
	error = m_textureMgr.init(m_renderer);
	if (error) return error;

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
	m_playerController.setShootCallback(std::bind(&Game::shoot, this));
}

void Game::run()
{
	//while()
	SDL_Event e;	

	m_timer.start();

	while (!m_input.check(Input::KESC)) {
		SDL_PollEvent(&e);
		m_input.read(e);
		float dt = m_timer.getDeltaTime();
		m_playerController.update(m_input, dt);		
		m_projectileComponentMgr.update(dt);
		draw();
	}
}

void Game::draw()
{
	SDL_RenderClear(m_renderer);
	SDL_Texture* textureEnemy = m_textureMgr.getTexture(TextureMgr::TextureID::ENEMY);
	SDL_Rect textureRect;
	textureRect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	textureRect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture
	auto positions = m_enemyComponentMgr.getPositions();
	for (size_t i = 0; i < ENEMY_COUNT; ++i) {
		textureRect.x = (int)positions[i].x;
		textureRect.y = (int)positions[i].y;
		SDL_RenderCopy(m_renderer, textureEnemy, nullptr, &textureRect);
	}

	SDL_Texture* texturePlayer = m_textureMgr.getTexture(TextureMgr::TextureID::PLAYER);
	auto playerPosition = m_player.getPosition();
	textureRect.x = (int)playerPosition.x;   // the x coordinate
	textureRect.y = (int)playerPosition.y;   // the y coordinate
	textureRect.w = (int)Player::SIZE;  // the width of the texture
	textureRect.h = (int)Player::SIZE;  // the height of the texture
	SDL_RenderCopy(m_renderer, texturePlayer, nullptr, &textureRect);

	SDL_Texture* texturePlayerBullet = m_textureMgr.getTexture(TextureMgr::TextureID::PLAYER_BULLET);
	auto projectilePositions = m_projectileComponentMgr.getPositions();
	size_t numberProjectiles = m_projectileComponentMgr.getSize();
	textureRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	textureRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	for (size_t i = 0; i < numberProjectiles; ++i) {
		textureRect.x = (int)projectilePositions[i].x;
		textureRect.y = (int)projectilePositions[i].y;
		SDL_RenderCopy(m_renderer, texturePlayerBullet, nullptr, &textureRect);
	}
	
	SDL_RenderPresent(m_renderer);
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

void Game::shoot()
{
	m_projectiles.push_back(m_entityManager.create());
	m_projectileComponentMgr.add(m_projectiles.back(), m_player.getPosition(), -100.0f);
}
