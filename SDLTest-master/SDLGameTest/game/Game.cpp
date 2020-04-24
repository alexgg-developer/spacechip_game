#include "Game.h"

#include "../dodf/MemoryPool.h"

#include "Vec.h"

#include <algorithm>
#include <random>
#include <string>

const float Game::INTERVAL_ENEMIES_SHOOT = 2.0f;

Game::Game(): m_playerController(m_player), m_timerEnemyShoot(0.0f)
{

}

Game::~Game()
{
	m_textureMgr.clean();
	m_textComponentMgr.clean();
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
	
	size_t initSize = 512 * 1024 * 1024; //512MB
	dodf::MemoryPool::Initialize(initSize);
	
	
	m_scoreMgr.init();

	initProjectiles();
	initEnemies(true);
	initObstacles(true);
	initPlayer();
	initUI(true);	

	error = m_textureMgr.init(m_renderer);
	if (error) return error;

	return error;
}

void Game::initProjectiles()
{
	const size_t PROJECTILE_MAX = 30u;
	m_projectileComponentMgr.allocate(PROJECTILE_MAX);
	m_playerProjectiles.reserve(PROJECTILE_MAX);

	using std::placeholders::_1;
	m_projectileComponentMgr.setDestroyCallback(std::bind(&Game::destroyProjectile, this, _1));
	//y = 0 on top
	m_projectileComponentMgr.setLimits(static_cast<float>(WINDOW_HEIGHT) - 30.0f, 30.0f);
}

void Game::initEnemies(bool allocate)
{
	constexpr size_t NUMBER_ROWS = 5u;
	constexpr size_t NUMBER_COLUMNS = 17u;
	constexpr size_t ENEMY_COUNT = NUMBER_COLUMNS * NUMBER_ROWS;
	if (allocate) {
		m_enemyComponentMgr.allocate(ENEMY_COUNT);
		m_enemies.reserve(ENEMY_COUNT);
	}
	const size_t TOP_MARGIN = 90u;
	const size_t ENEMY_SIZE = EnemyComponentMgr::ENEMY_SIZE; //35
	const size_t HORIZONTAL_MARGIN_BETWEEN_ENEMIES = 3u;
	const size_t VERTICAL_MARGIN_BETWEEN_ENEMIES = 3u;
	const size_t LEFT_MARGIN = (WINDOW_WIDTH - NUMBER_COLUMNS * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES)) / 2u;
	for (size_t i = 0; i < ENEMY_COUNT; ++i) {
		m_enemies.push_back(m_entityManager.create());
		vec2 position;
		size_t horizontalIndex = i % NUMBER_COLUMNS;
		position.x = static_cast<float>(LEFT_MARGIN + horizontalIndex * (ENEMY_SIZE + HORIZONTAL_MARGIN_BETWEEN_ENEMIES));
		size_t verticalIndex = i / NUMBER_COLUMNS;
		position.y = static_cast<float>(TOP_MARGIN + verticalIndex * (ENEMY_SIZE + VERTICAL_MARGIN_BETWEEN_ENEMIES));
		m_enemyComponentMgr.add(m_enemies[i], position);
	}		
	const size_t HORIZONTAL_LIMIT = 5u;
	m_enemyComponentMgr.setLimits((float)HORIZONTAL_LIMIT, (float)(WINDOW_WIDTH - HORIZONTAL_LIMIT - ENEMY_SIZE));
	m_enemyComponentMgr.setSpeed(55.0f);
	//m_enemyComponentMgr.setSpeed(225.0f);
}

void Game::initObstacles(bool allocate)
{
	if(allocate) m_obstacleComponentMgr.allocate(50u);

	const size_t OBSTACLE_COUNT = 5u;
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
	for (size_t i = 0; i < obstacleCount; ++i) {
		Entity obstacle = m_entityManager.create();
		vec2 position;
		position.x = static_cast<float>(horizontalDis(gen));
		position.y = static_cast<float>(verticalDis(gen));
		m_obstacleComponentMgr.add(obstacle, position);
	}
}

void Game::initPlayer()
{
	vec2 position;
	position.x = 70.0f;
	position.y = static_cast<float>(WINDOW_HEIGHT - 100u);
	m_player.setPosition(position);
	m_playerController.setLimits(30.0f, static_cast<float>(WINDOW_WIDTH) - 30.0f - static_cast<float>(Player::SIZE));
	m_playerController.setShootCallback(std::bind(&Game::shoot, this));
}

void Game::run()
{
	m_timer.start();
	SDL_Event e;
	while (!m_input.check(Input::KESC)) {
		SDL_PollEvent(&e);
		m_input.read(e);
		switch (m_state) {
		case GameState::DEFAULT: {
			float dt = m_timer.getDeltaTime();
			m_playerController.update(m_input, dt);
			m_projectileComponentMgr.update(dt);
			m_projectileComponentMgr.checkOffLimits();
			m_enemyComponentMgr.update(dt);
			m_enemyComponentMgr.checkLimits();
			checkCollissionsEnemyWithPlayer();
			checkCollisionsPlayerProjectile();
			checkCollisionsEnemyProjectiles();

			m_timerEnemyShoot += dt;
			if (m_timerEnemyShoot >= INTERVAL_ENEMIES_SHOOT) {
				shootEnemy();
				m_timerEnemyShoot = 0.0f;
			}

			draw();
		}
		break;

		case Game::GAME_OVER:
			if (m_input.checkPressed(Input::KENTER)) restart();
			drawUI();
		break;
		
		case Game::WIN:
			if (m_input.checkPressed(Input::KENTER)) restart();
			drawUI();
		break;
		}
	}
}

void Game::draw()
{
	SDL_RenderClear(m_renderer);

	SDL_Texture* textureEnemy = m_textureMgr.getTexture(TextureMgr::TextureID::ENEMY);
	SDL_Rect textureRect;
	textureRect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	textureRect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture
	auto xCoords = m_enemyComponentMgr.getXCoords();
	auto yCoords = m_enemyComponentMgr.getYCoords();
	auto size = m_enemyComponentMgr.getSize();
	for (size_t i = 0; i < size; ++i) {
		textureRect.x = (int)xCoords[i];
		textureRect.y = (int)yCoords[i];
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
	xCoords = m_projectileComponentMgr.getXCoords();
	yCoords = m_projectileComponentMgr.getYCoords();
	auto textureIDs = m_projectileComponentMgr.getTextureIDs();
	size_t numberProjectiles = m_projectileComponentMgr.getSize();
	textureRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	textureRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;  
	for (size_t i = 0; i < numberProjectiles; ++i) {
		textureRect.x = (int)xCoords[i];
		textureRect.y = (int)yCoords[i];
		SDL_RenderCopy(m_renderer, textures[textureIDs[i]], nullptr, &textureRect);
	}
	
	SDL_Texture* textureObstacle = m_textureMgr.getTexture(TextureMgr::TextureID::OBSTACLE);
	textureRect.w = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the width of the texture
	textureRect.h = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the height of the texture
	xCoords = m_obstacleComponentMgr.getXCoords();
	yCoords = m_obstacleComponentMgr.getYCoords();
	size = m_obstacleComponentMgr.getSize();
	for (size_t i = 0; i < size; ++i) {
		textureRect.x = (int)xCoords[i];
		textureRect.y = (int)yCoords[i];
		SDL_RenderCopy(m_renderer, textureObstacle, nullptr, &textureRect);
	}

	SDL_Texture** textTextures = m_textComponentMgr.getTextures();
	vec2* positions_ = m_textComponentMgr.getPositions();
	size = m_textComponentMgr.getSize();
	uint32_t* width = m_textComponentMgr.getWidths();
	uint32_t* height = m_textComponentMgr.getHeights();
	for (size_t i = 0; i < size; ++i) {
		textureRect.x = (int)positions_[i].x;
		textureRect.y = (int)positions_[i].y;
		textureRect.w = (int)width[i];  // the width of the texture
		textureRect.h = (int)height[i];  // the height of the texture
		SDL_RenderCopy(m_renderer, textTextures[i], nullptr, &textureRect);
	}

	SDL_RenderPresent(m_renderer);
}


void Game::drawUI()
{
	SDL_RenderClear(m_renderer);

	SDL_Texture** textTextures = m_textComponentMgr.getTextures();
	vec2* positions_ = m_textComponentMgr.getPositions();
	size_t size = m_textComponentMgr.getSize();
	uint32_t* width = m_textComponentMgr.getWidths();
	uint32_t* height = m_textComponentMgr.getHeights();
	SDL_Rect textureRect;
	for (size_t i = 0; i < size; ++i) {
		textureRect.x = (int)positions_[i].x;
		textureRect.y = (int)positions_[i].y;
		textureRect.w = (int)width[i];  // the width of the texture
		textureRect.h = (int)height[i];  // the height of the texture
		SDL_RenderCopy(m_renderer, textTextures[i], nullptr, &textureRect);
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

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 1;
	}

	return 0;
}

void Game::initUI(bool allocate)
{
	const size_t TEXT_MAX = 30u;
	if(allocate) m_textComponentMgr.init(TEXT_MAX);

	m_textComponentMgr.add(m_entityManager.create(), vec2{ 20u, 10u }, "Score: ", m_renderer);
	m_currentScoreText = m_entityManager.create();
	m_textComponentMgr.add(m_currentScoreText, vec2{ 110u, 10u }, std::to_string(m_scoreMgr.getCurrentScore()), m_renderer);

	m_textComponentMgr.add(m_entityManager.create(), vec2{ WINDOW_WIDTH - 225u, 10 }, "High score: ", m_renderer);
	m_maxScoreText = m_entityManager.create();
	m_textComponentMgr.add(m_maxScoreText, vec2{ WINDOW_WIDTH - 65u, 10 }, std::to_string(m_scoreMgr.getHighScore()), m_renderer);

	m_textComponentMgr.add(m_entityManager.create(), vec2{ 10u, WINDOW_HEIGHT - 30u }, "Life: ", m_renderer);
	m_lifeText = m_entityManager.create();
	m_textComponentMgr.add(m_lifeText, vec2{ 100u, WINDOW_HEIGHT - 30u }, std::to_string(m_player.getLife()), m_renderer);
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
	
	vec2 position = m_enemyComponentMgr.getPosition(m_enemies[dis(gen)]);
	position.x += EnemyComponentMgr::ENEMY_SIZE / 2u;
	position.y += EnemyComponentMgr::ENEMY_SIZE;
	m_projectileComponentMgr.add(m_enemyProjectiles.back(), 
		position, 
		300.0f, 
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
	float* xCoordsProjectile = m_projectileComponentMgr.getXCoords();
	float* yCoordsProjectile = m_projectileComponentMgr.getYCoords();
	SDL_Rect projectileRect;
	projectileRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	projectileRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	
	for (intptr_t i = m_playerProjectiles.size() - 1; i >= 0; --i) {
		Instance projectileInstance = m_projectileComponentMgr.lookup(m_playerProjectiles[i]);
		projectileRect.x = (int)xCoordsProjectile[projectileInstance.i];
		projectileRect.y = (int)yCoordsProjectile[projectileInstance.i];

		Entity collided = m_obstacleComponentMgr.checkShot(projectileRect);
		if (collided.isValid()) {
			Instance obstacleInstance = m_obstacleComponentMgr.lookup(collided);
			auto life = m_obstacleComponentMgr.getLife(obstacleInstance) - 1;
			if (life != 0) {
				m_obstacleComponentMgr.setLife(obstacleInstance, life);				
			}
			else {
				m_scoreMgr.addScore(ScoreMgr::ScoreID::OBSTACLE);
				m_textComponentMgr.setText(m_currentScoreText, std::to_string(m_scoreMgr.getCurrentScore()), m_renderer);
				m_obstacleComponentMgr.destroy(collided);
				m_entityManager.destroy(collided);
			}

			m_projectileComponentMgr.destroy(projectileInstance);
			m_entityManager.destroy(m_playerProjectiles[i]);
			m_playerProjectiles.erase(m_playerProjectiles.begin() + i);
		}
		else {
			collided = m_enemyComponentMgr.checkShot(projectileRect);
			if (collided.isValid()) {
				Instance enemyInstance = m_enemyComponentMgr.lookup(collided);
				auto life = m_enemyComponentMgr.getLife(enemyInstance) - 1;
				if (life != 0) {
					m_enemyComponentMgr.setLife(enemyInstance, life);
				}
				else {
					m_scoreMgr.addScore(ScoreMgr::ScoreID::ENEMY);
					m_textComponentMgr.setText(m_currentScoreText, std::to_string(m_scoreMgr.getCurrentScore()), m_renderer);
					m_enemies.erase(std::lower_bound(m_enemies.begin(), m_enemies.end(), collided));
					m_enemyComponentMgr.destroy(collided);
					m_entityManager.destroy(collided);
				}
				m_projectileComponentMgr.destroy(projectileInstance);
				m_entityManager.destroy(m_playerProjectiles[i]);
				m_playerProjectiles.erase(m_playerProjectiles.begin() + i);
			}
		}
	}
}


void Game::checkCollisionsEnemyProjectiles()
{
	float* xCoordsProjectile = m_projectileComponentMgr.getXCoords();
	float* yCoordsProjectile = m_projectileComponentMgr.getYCoords();
	SDL_Rect projectileRect, playerRect;
	playerRect.x = (int)m_player.getPosition().x;
	playerRect.y = (int)m_player.getPosition().y;
	playerRect.w = (int)Player::SIZE;  // the width of the texture
	playerRect.h = (int)Player::SIZE;  // the height of the texture

	projectileRect.w = (int)ProjectileComponentMgr::PROJECTILE_SIZE;
	projectileRect.h = (int)ProjectileComponentMgr::PROJECTILE_SIZE;

	for (intptr_t i = m_enemyProjectiles.size() - 1; i >= 0; --i) {
		Instance projectileInstance = m_projectileComponentMgr.lookup(m_enemyProjectiles[i]);
		projectileRect.x = (int)xCoordsProjectile[projectileInstance.i];
		projectileRect.y = (int)yCoordsProjectile[projectileInstance.i];
		if (SDL_HasIntersection(&projectileRect, &playerRect)) {
			int32_t life = m_player.getLife() - 1;
			m_textComponentMgr.setText(m_lifeText, std::to_string(life), m_renderer);
			m_player.setLife(life);
			if (life != 0) {
				m_projectileComponentMgr.destroy(projectileInstance);
				m_entityManager.destroy(m_enemyProjectiles[i]);
				m_enemyProjectiles.pop_back();
			}
			else {
				gameOver();
				break;
			}
		}
	}
}

void Game::checkCollissionsEnemyWithPlayer()
{
	//vec2* enemyPositions = m_enemyComponentMgr.getPositions();
	float* enemyXCoords = m_enemyComponentMgr.getXCoords();
	float* enemyYCoords = m_enemyComponentMgr.getYCoords();
	size_t enemyInstances = m_enemyComponentMgr.getSize();
	SDL_Rect enemyRect, playerRect;
	playerRect.x = (int)m_player.getPosition().x;
	playerRect.y = (int)m_player.getPosition().y;
	playerRect.w = (int)Player::SIZE;  // the width of the texture
	playerRect.h = (int)Player::SIZE;  // the height of the texture

	enemyRect.w = (int)EnemyComponentMgr::ENEMY_SIZE;
	enemyRect.h = (int)EnemyComponentMgr::ENEMY_SIZE;

	for (size_t i = 0; i < enemyInstances; ++i) {
		enemyRect.x = (int)enemyXCoords[i];
		enemyRect.y = (int)enemyYCoords[i];
		if (SDL_HasIntersection(&enemyRect, &playerRect)) {
			m_player.setLife(0);			
			gameOver();
			break;
		}
	}
}

void Game::gameOver()
{
	m_state = GameState::GAME_OVER;
	m_textComponentMgr.add(m_entityManager.create(), vec2{ WINDOW_WIDTH/2u - 100u, WINDOW_HEIGHT/2u - 100u }, "Game over", m_renderer);
	m_textComponentMgr.add(m_entityManager.create(), vec2{ WINDOW_WIDTH/2u - 100u, WINDOW_HEIGHT/2u - 50u}, "Press enter to restart", m_renderer);

	m_scoreMgr.saveHighScore();
}


void Game::win()
{
	m_state = GameState::WIN;
	m_textComponentMgr.add(m_entityManager.create(), vec2{ WINDOW_WIDTH / 2u - 100u, WINDOW_HEIGHT / 2u - 100u }, "You win!", m_renderer);
	m_textComponentMgr.add(m_entityManager.create(), vec2{ WINDOW_WIDTH / 2u - 100u, WINDOW_HEIGHT / 2u - 50u }, "Press enter to restart", m_renderer);

	m_scoreMgr.saveHighScore();
}

void Game::restart()
{
	m_entityManager.reset();

	m_enemyComponentMgr.reset();
	m_enemies.clear();
	initEnemies(false);

	m_obstacleComponentMgr.reset();
	initObstacles(false);

	m_projectileComponentMgr.reset();
	m_playerProjectiles.clear();
	m_enemyProjectiles.clear();
	
	m_player.reset();
	m_scoreMgr.reset();

	m_textComponentMgr.reset();
	initUI(false);

	m_timer.restart();
	m_state = GameState::DEFAULT;
}
