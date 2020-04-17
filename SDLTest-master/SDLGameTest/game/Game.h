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

#include "../dodf/Entity.h"
#include "../dodf/EntityManager.h"

class Game
{
public: 
	Game();
	void draw();
	int init();
	void run();

private:
	static const size_t ENEMY_COUNT = 110;

	SDL_Renderer * m_renderer;
	SDL_Window * m_window;

	EntityManager m_entityManager;
	EnemyComponentMgr m_enemyComponentMgr;
	//std::array<Entity, ENEMY_COUNT> m_enemies;
	//std::vector<Entity> m_enemies;
	Entity* m_enemies;

	void initEnemies();
	int initSDL();
};
