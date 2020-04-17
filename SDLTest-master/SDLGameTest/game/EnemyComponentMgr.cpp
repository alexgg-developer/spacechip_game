#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "EnemyComponentMgr.h"

const float EnemyComponentMgr::ENEMY_SIZE = 60.0f;

void EnemyComponentMgr::clean()
{

}


void EnemyComponentMgr::allocate(size_t size)
{
	//const size_t bytes = size * sizeof(float);
	//m_buffer.data = MemoryPool::Get(bytes);
	//m_buffer.allocated = size;
	m_data.life = static_cast<size_t*>(MemoryPool::Get(size * sizeof(size_t)));
	m_data.position = static_cast<glm::vec3*>(MemoryPool::Get(size * sizeof(glm::vec3)));

	//m_buffer.n = 0;
}

void EnemyComponentMgr::add(const Entity& e, const glm::vec3& position)
{
	auto instance = Instance::create(m_instanceCount);
	//m_map[e.index()] = instance;
	m_data.life[instance.i] = rand() % 3u + 1u;
	m_data.position[instance.i] = position;
	++m_instanceCount;
}
