#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "ObstacleComponentMgr.h"


using namespace dodf;

const size_t ObstacleComponentMgr::OBSTACLE_SIZE = 55;

void ObstacleComponentMgr::clean()
{

}

void ObstacleComponentMgr::destroy(const Entity & e)
{
	size_t last = m_instanceCount - 1;
	auto index = e.index();
	Instance i = lookup(e);
	m_map.erase(e.index());

	Entity lastEntity = m_data.entity[last];

	m_data.entity[i.i] = m_data.entity[last];
	m_data.life[i.i] = m_data.life[last];
	m_data.position[i.i] = m_data.position[last];

	m_map[lastEntity.index()] = i;

	--m_instanceCount;
}

Entity ObstacleComponentMgr::checkShot(const SDL_Rect & rect)
{
	Entity entityShot;
	SDL_Rect textureRect;
	textureRect.w = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the width of the texture
	textureRect.h = (int)ObstacleComponentMgr::OBSTACLE_SIZE;  // the height of the texture
	for (size_t i = 0; i < m_instanceCount; ++i) {
		textureRect.x = (int)m_data.position[i].x;
		textureRect.y = (int)m_data.position[i].y;
		if (SDL_HasIntersection(&rect, &textureRect)) {
			entityShot = m_data.entity[i];
			break;
		}
	}

	return entityShot;
}


void ObstacleComponentMgr::allocate(size_t size)
{
	m_data.life = static_cast<size_t*>(MemoryPool::Get(size * sizeof(size_t)));
	m_data.position = static_cast<vec3*>(MemoryPool::Get(size * sizeof(vec3)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));

	m_instanceCount = 0;
}

void ObstacleComponentMgr::add(const Entity& e, const vec3& position)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.life[instance.i] = rand() % 5u + 1u;
	m_data.position[instance.i] = position;
	m_data.entity[instance.i] = e;

	++m_instanceCount;
}

void ObstacleComponentMgr::setDestroyCallback(std::function<void(Entity)> callback)
{
	m_destroyCallback = callback;
}