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
#include "Player.h"
#include "Input.h"
#include "Timer.h"
#include "PlayerController.h"
#include "TextureMgr.h"

#include "../dodf/Entity.h"
#include "../dodf/EntityManager.h"

class Game
{
public: 
	Game();
	~Game();
	void draw();
	int init();
	void run();

private:
	static const size_t ENEMY_COUNT = 110;
	static const size_t WINDOW_WIDTH = 800;
	static const size_t WINDOW_HEIGHT = 600;
	const size_t MAXIMUM_SHOTS_PLAYER = 3;

	SDL_Renderer * m_renderer;
	SDL_Window * m_window;

	EntityManager m_entityManager;
	EnemyComponentMgr m_enemyComponentMgr;
	ProjectileComponentMgr m_projectileComponentMgr;
	//std::array<Entity, ENEMY_COUNT> m_enemies;
	//std::vector<Entity> m_enemies;
	std::vector<Entity> m_playerProjectiles;
	Entity* m_enemies;
	Player m_player;
	Input m_input;
	Timer m_timer;
	PlayerController m_playerController;
	TextureMgr m_textureMgr;

	void checkCollisions();
	void destroyProjectile(Entity e);
	void initEnemies();
	void initPlayer();
	int initSDL();
	void shoot();
};
