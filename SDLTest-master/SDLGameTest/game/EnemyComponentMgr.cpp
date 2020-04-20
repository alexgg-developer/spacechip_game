#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "EnemyComponentMgr.h"


using namespace dodf;

const size_t EnemyComponentMgr::ENEMY_SIZE = 30;

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
	m_data.position[i.i] = m_data.position[last];

	m_map[lastEntity.index()] = i;

	--m_instanceCount;
}

Entity EnemyComponentMgr::checkShot(const SDL_Rect & rect)
{
	Entity entityShot;
	SDL_Rect textureRect;
	textureRect.w = (int)EnemyComponentMgr::ENEMY_SIZE;  // the width of the texture
	textureRect.h = (int)EnemyComponentMgr::ENEMY_SIZE;  // the height of the texture
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


void EnemyComponentMgr::allocate(size_t size)
{
	//const size_t bytes = size * sizeof(float);
	//m_buffer.data = MemoryPool::Get(bytes);
	//m_buffer.allocated = size;
	m_data.life = static_cast<size_t*>(MemoryPool::Get(size * sizeof(size_t)));
	m_data.position = static_cast<vec3*>(MemoryPool::Get(size * sizeof(vec3)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));

	//m_buffer.n = 0;	
}

void EnemyComponentMgr::add(const Entity& e, const vec3& position)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.life[instance.i] = rand() % 3u + 1u;
	m_data.position[instance.i] = position;
	m_data.entity[instance.i] = e;
	++m_instanceCount;
}

