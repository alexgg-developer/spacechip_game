#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#include <SDL_image.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#endif

#include <iostream>
#include <array>

#include "EnemyComponentMgr.h"
#include "ProjectileComponentMgr.h"
#include "ObstacleComponentMgr.h"
#include "TextComponentMgr.h"
#include "Player.h"
#include "Input.h"
#include "Timer.h"
#include "PlayerController.h"
#include "TextureMgr.h"
#include "ScoreMgr.h"

#include "../dodf/Entity.h"
#include "../dodf/EntityManager.h"

class Game
{
public: 
	Game();
	~Game();
	int init();
	void run();

private:
	static constexpr size_t WINDOW_WIDTH = 800u;
	static constexpr size_t WINDOW_HEIGHT = 600u;
	static const size_t MAXIMUM_SHOTS_PLAYER = 3;
	static const float INTERVAL_ENEMIES_SHOOT;

	enum GameState : uint32_t {
		DEFAULT,
		GAME_OVER,
		WIN,
		SIZE
	};


	SDL_Renderer * m_renderer;
	SDL_Window * m_window;

	EnemyComponentMgr m_enemyComponentMgr;
	ObstacleComponentMgr m_obstacleComponentMgr;
	ProjectileComponentMgr m_projectileComponentMgr;	
	TextComponentMgr m_textComponentMgr;
	std::vector<Entity> m_playerProjectiles;
	std::vector<Entity> m_enemyProjectiles;
	std::vector<Entity> m_enemies;
	Entity m_maxScoreText, m_currentScoreText, m_lifeText;
	Player m_player;
	Input m_input;
	Timer m_timer;
	PlayerController m_playerController;
	EntityManager m_entityManager;
	ScoreMgr m_scoreMgr;
	TextureMgr m_textureMgr;

	float m_timerEnemyShoot;
	GameState m_state = GameState::DEFAULT;

	void checkCollisionsPlayerProjectile();
	void checkCollisionsEnemyProjectiles();
	void checkCollissionsEnemyWithPlayer();
	void destroyProjectile(Entity e);
	void draw();
	void drawUI();
	void gameOver();
	void initEnemies(bool allocate);
	void initPlayer();
	void initProjectiles();
	void initObstacles(bool allocate);
	int initSDL();
	void initUI(bool allocate);
	void restart();
	void shoot();
	void shootEnemy();
	void win();
};
