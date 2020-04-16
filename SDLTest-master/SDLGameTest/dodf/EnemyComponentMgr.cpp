#include "EnemyComponentMgr.h"
#include "Assert.h"
#include "MemoryPool.h"

#include <glm/gtc/matrix_transform.hpp>

void EnemyComponentMgr::clean()
{

}


void EnemyComponentMgr::allocate(size_t size)
{
	const size_t bytes = size * sizeof(float);
	m_buffer.data = MemoryPool::Get(bytes);
	m_buffer.allocated = size;
	m_data.life = static_cast<size_t*>(m_buffer.data);

	m_buffer.n = 0;
}

void EnemyComponentMgr::add(const Entity& e)
{
	ASSERT(m_buffer.n + 1 < m_buffer.allocated);
	auto instance = Instance::create(m_buffer.n);
	m_map[e.index()] = instance;
	++m_buffer.n;
}