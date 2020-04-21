#include "Game.h"

#include "../dodf/MemoryPool.h"

#include "Vec.h"

#include <algorithm>
#include <random>

const float Game::INTERVAL_ENEMIES_SHOOT = 2.0f;

Game::Game(): m_playerController(m_player), m_timerEnemyShoot(0.0f)
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
	//y = 0 on top
	m_projectileComponentMgr.setLimits(static_cast<float>(WINDOW_HEIGHT) - 30.0f, 30.0f);
	Entity e;
	using std::placeholders::_1;
	m_projectileComponentMgr.setDestroyCallback(std::bind(&Game::destroyProjectile, this, _1));
	m_playerProjectiles.reserve(projectileMax);
	initEnemies();
	initObstacles();
	initPlayer();	
	error = m_textureMgr.init(m_renderer);
	if (error) return error;

	return error;
}

void Game::initEnemies()
{
	constexpr size_t NUMBER_ROWS = 5u;
	constexpr size_t NUMBER_COLUMNS = 17u;
	constexpr size_t ENEMY_COUNT = NUMBER_COLUMNS * NUMBER_ROWS;
	m_enemyComponentMgr.allocate(ENEMY_COUNT);
	m_enemies.reserve(ENEMY_COUNT);
	//const size_t LEFT_MARGIN = 70u;
	const size_t TOP_MARGIN = 90u;
	const size_t ENEMY_SIZE = EnemyComponentMgr::ENEMY_SIZE; //35
	const size_t HORIZONTAL_MARGIN_BETWEEN_ENEMIES = 3u;
	const size_t VERTICAL_MARGIN_BETWEEN_ENEMIES = 3u;
	const size_t LEFT_MARGIN = (WINDOW_WIDTH - NUMBER_COLUMNS * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES)) / 2u;
	for (size_t i = 0; i < ENEMY_COUNT; ++i) {
		m_enemies.push_back(m_entityManager.create());
		vec3 position;
		//position.x = i * EnemyComponentMgr::ENEMY_SIZE;
		size_t horizontalIndex = i % NUMBER_COLUMNS;
		position.x = static_cast<float>(LEFT_MARGIN + horizontalIndex * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES));
		size_t verticalIndex = i / NUMBER_COLUMNS;
		position.y = static_cast<float>(TOP_MARGIN + verticalIndex * (ENEMY_SIZE + VERTICAL_MARGIN_BETWEEN_ENEMIES));
		position.z = 0.0f;
		m_enemyComponentMgr.add(m_enemies[i], position);
	}		
	const size_t HORIZONTAL_LIMIT = 5u;
	m_enemyComponentMgr.setLimits((float)HORIZONTAL_LIMIT, (float)(WINDOW_WIDTH - HORIZONTAL_LIMIT - ENEMY_SIZE));
	m_enemyComponentMgr.setSpeed(25.0f);
}

void Game::initObstacles()
{
	m_obstacleComponentMgr.allocate(50u);

	const size_t OBSTACLE_COUNT = 1u;
	const size_t OBSTACLE_MULTIPLIER = 2u;
	constexpr size_t HORIZONTAL_MARGIN = 70u;
	const size_t TOP_MARGIN = 90u;
	constexpr size_t BOTTOM_MARGIN = WINDOW_HEIGHT - TOP_MARGIN * 2u;
	const size_t ENEMY_SIZE = EnemyComponentMgr::ENEMY_SIZE;

	size_t obstacleCount = OBSTACLE_COUNT;

	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> horizontalDis(HORIZONTAL_MARGIN, WINDOW_WIDTH - HORIZONTAL_MARGIN);
	std::uniform_int_distribution<> verticalDis(TOP_MARGIN, BOTTOM_MARGIN);
	//constexpr size_t HORIZONTAL_SPACE = WINDOW_WIDTH - HORIZONTAL_MARGIN * 2u;
	for (size_t i = 0; i < obstacleCount; ++i) {
		Entity obstacle = m_entityManager.create();
		vec3 position;
		position.x = static_cast<float>(horizontalDis(gen));
		position.y = static_cast<float>(verticalDis(gen));
		position.z = 0.0f;
		m_obstacleComponentMgr.add(obstacle, position);
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
		m_projectileComponentMgr.checkOffLimits();
		m_enemyComponentMgr.update(dt);
		m_enemyComponentMgr.checkLimits();
		checkCollisionsPlayerProjectile();
		checkCollisionsEnemyProjectiles();

		m_timerEnemyShoot += dt;
		if (m_timerEnemyShoot >= INTERVAL_ENEMIES_SHOOT) {
			shootEnemy();
			m_timerEnemyShoot = 0.0f;
		}

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
	auto size = m_enemyComponentMgr.getSize();
	for (size_t i = 0; i < size; ++i) {
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


	SDL_Texture** textures = m_textureMgr.getTextures();
	auto projectilePositions = m_projectileComponentMgr.getPositions();
	auto textureIDs = m_projectileComponentMgr.getTextureIDs();
	size_t numberProjectiles = m_projectileComponentMgr.getSize();
	textureRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	textureRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	for (size_t i = 0; i < numberProjectiles; ++i) {
		textureRect.x = (int)projectilePositions[i].x;
		textureRect.y = (int)projectilePositions[i].y;
		SDL_RenderCopy(m_renderer, textures[textureIDs[i]], nullptr, &textureRect);
	}
	
	SDL_Texture* textureObstacle = m_textureMgr.getTexture(TextureMgr::TextureID::OBSTACLE);
	textureRect.w = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the width of the texture
	textureRect.h = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the height of the texture
	positions = m_obstacleComponentMgr.getPositions();
	size = m_obstacleComponentMgr.getSize();
	for (size_t i = 0; i < size; ++i) {
		textureRect.x = (int)positions[i].x;
		textureRect.y = (int)positions[i].y;
		SDL_RenderCopy(m_renderer, textureObstacle, nullptr, &textureRect);
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
	if (m_playerProjectiles.size() < MAXIMUM_SHOTS_PLAYER) {
		m_playerProjectiles.push_back(m_entityManager.create());
		m_projectileComponentMgr.add(m_playerProjectiles.back(), m_player.getPosition(), -250.0f, TextureMgr::TextureID::PLAYER_BULLET);
	}
}

void Game::shootEnemy()
{
	m_enemyProjectiles.push_back(m_entityManager.create());
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, static_cast<int>(m_enemies.size()) - 1);

	m_projectileComponentMgr.add(m_enemyProjectiles.back(), 
		m_enemyComponentMgr.getPosition(m_enemies[dis(gen)]), 
		250.0f, 
		TextureMgr::TextureID::ENEMY_BULLET);
}

void Game::destroyProjectile(Entity e)
{
	auto it = std::find(m_playerProjectiles.begin(), m_playerProjectiles.end(), e);
	if (it != m_playerProjectiles.end()) {
		m_playerProjectiles.erase(it);
	}
	else {
		auto it = std::find(m_enemyProjectiles.begin(), m_enemyProjectiles.end(), e);
		if (it != m_enemyProjectiles.end()) {
			m_enemyProjectiles.erase(it);
		}
	}
	m_entityManager.destroy(e);
}

void Game::checkCollisionsPlayerProjectile()
{
	vec3* projectilePositions = m_projectileComponentMgr.getPositions();
	//size_t numberProjectiles = m_projectileComponentMgr.getSize();
	SDL_Rect projectileRect;
	projectileRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	projectileRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	
	for (intptr_t i = m_playerProjectiles.size() - 1; i >= 0; --i) {
		Instance projectileInstance = m_projectileComponentMgr.lookup(m_playerProjectiles[i]);
		projectileRect.x = (int)projectilePositions[projectileInstance.i].x;
		projectileRect.y = (int)projectilePositions[projectileInstance.i].y;

		Entity collided = m_obstacleComponentMgr.checkShot(projectileRect);
		if (collided.isValid()) {
			m_obstacleComponentMgr.destroy(collided);
			m_entityManager.destroy(collided);
			m_projectileComponentMgr.destroy(projectileInstance);
			m_entityManager.destroy(m_playerProjectiles[i]);
			m_playerProjectiles.erase(m_playerProjectiles.begin() + i);
		}
		else {
			collided = m_enemyComponentMgr.checkShot(projectileRect);
			if (collided.isValid()) {
				m_enemies.erase(std::lower_bound(m_enemies.begin(), m_enemies.end(), collided));
				m_enemyComponentMgr.destroy(collided);
				m_entityManager.destroy(collided);
				m_projectileComponentMgr.destroy(projectileInstance);
				m_entityManager.destroy(m_playerProjectiles[i]);
				m_playerProjectiles.erase(m_playerProjectiles.begin() + i);
			}
		}
	}
}


void Game::checkCollisionsEnemyProjectiles()
{
	vec3* projectilePositions = m_projectileComponentMgr.getPositions();
	SDL_Rect projectileRect, playerRect;
	playerRect.x = (int)m_player.getPosition().x;
	playerRect.y = (int)m_player.getPosition().y;
	playerRect.w = (int)Player::SIZE;  // the width of the texture
	playerRect.h = (int)Player::SIZE;  // the height of the texture

	projectileRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	projectileRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;

	for (intptr_t i = m_enemyProjectiles.size() - 1; i >= 0; --i) {
		Instance projectileInstance = m_projectileComponentMgr.lookup(m_enemyProjectiles[i]);
		projectileRect.x = (int)projectilePositions[projectileInstance.i].x;
		projectileRect.y = (int)projectilePositions[projectileInstance.i].y;
		if (SDL_HasIntersection(&projectileRect, &playerRect)) {
			m_player.setLife(m_player.getLife() - 1);
			std::cout << "player life down::" << m_player.getLife() << std::endl;
			m_projectileComponentMgr.destroy(projectileInstance);
			m_entityManager.destroy(m_enemyProjectiles[i]);
			m_enemyProjectiles.pop_back();
		}
	}
}

