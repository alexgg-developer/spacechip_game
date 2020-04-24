#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "EnemyComponentMgr.h"

#include <cmath>

using namespace dodf;

const size_t EnemyComponentMgr::ENEMY_SIZE = 36;

void EnemyComponentMgr::clean()
{

}

void EnemyComponentMgr::destroy(const Entity & e)
{
	size_t last = m_instanceCount - 1;
	auto index = e.index();
	Instance i = lookup(e);
	m_map.erase(e.index());

	Entity lastEntity = m_data.entity[last];

	m_data.entity[i.i] = m_data.entity[last];
	m_data.life[i.i] = m_data.life[last];
	m_data.x[i.i] = m_data.x[last];
	m_data.y[i.i] = m_data.y[last];

	m_map[lastEntity.index()] = i;

	--m_instanceCount;
}

void EnemyComponentMgr::reset()
{
	memset(m_data.life, 0, m_capacity * sizeof(int32_t));
	memset(m_data.x, 0, m_capacity * sizeof(float));
	memset(m_data.y, 0, m_capacity * sizeof(float));
	memset(m_data.entity, 0, m_capacity * sizeof(Entity));
	
	m_map.clear();
	m_instanceCount = 0;
}

void EnemyComponentMgr::setSpeed(float speed)
{
	m_speed = speed;
}

void EnemyComponentMgr::setLimits(float leftLimit, float rightLimit)
{
	m_leftLimit = leftLimit;
	m_rightLimit = rightLimit;
}

Entity EnemyComponentMgr::checkShot(const SDL_Rect& rect)
{
	Entity entityShot;
	SDL_Rect textureRect;
	textureRect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	textureRect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture
	for (size_t i = 0; i < m_instanceCount; ++i) {
		textureRect.x = (int)m_data.x[i];
		textureRect.y = (int)m_data.y[i];
		if (SDL_HasIntersection(&rect, &textureRect)) {
			entityShot = m_data.entity[i];
			break;
		}
	}

	return entityShot;
}


void EnemyComponentMgr::allocate(size_t size)
{
	m_data.life = static_cast<int32_t*>(MemoryPool::Get(size * sizeof(int32_t)));
	m_data.x = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.y = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));
	
	m_instanceCount = 0;
	m_capacity = size;
}

void EnemyComponentMgr::add(const Entity& e, const vec2& position)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.life[instance.i] = rand() % 3u + 1u;
	m_data.x[instance.i] = position.x;
	m_data.y[instance.i] = position.y;
	m_data.entity[instance.i] = e;
	++m_instanceCount;
}

void EnemyComponentMgr::update(float dt)
{
	for (size_t i = 0; i < m_instanceCount; ++i) {
		m_data.x[i] += m_speed * dt;
	}
}

void EnemyComponentMgr::checkLimits()
{
	bool hasToMoveDown = false;
	for (size_t i = 0; i < m_instanceCount; ++i) {
		if (m_data.x[i] < m_leftLimit || m_data.x[i] > m_rightLimit) {
			hasToMoveDown = true;
			break;
		}
	}

	if (hasToMoveDown) {
		m_speed = -m_speed;
		float enemySize = (float)ENEMY_SIZE;
		for (size_t i = 0; i < m_instanceCount; ++i) {
			m_data.y[i] += enemySize * 0.3f;
			m_data.x[i] += enemySize * 0.1f * copysignf(1.0f, m_speed);
		}
	}
}


void EnemyComponentMgr::setLife(Instance i, int32_t life)
{
	ASSERT(i.i < m_instanceCount);
	m_data.life[i.i] = life;
}



