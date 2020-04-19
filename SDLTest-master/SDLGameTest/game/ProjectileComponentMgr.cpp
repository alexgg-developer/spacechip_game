#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "ProjectileComponentMgr.h"


using namespace dodf;


const size_t ProjectileComponentMgr::PROJECTILE_SIZE = 15;

void ProjectileComponentMgr::allocate(size_t size)
{
	m_data.position = static_cast<vec3*>(MemoryPool::Get(size * sizeof(vec3)));
	m_data.speed = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
}

void ProjectileComponentMgr::add(const Entity& e, const vec3 & position, float speed)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.position[instance.i] = position;
	m_data.speed[instance.i] = speed;
	++m_instanceCount;
}

void ProjectileComponentMgr::update(float dt)
{
	for (size_t i = 0; i < m_instanceCount; ++i) {
		m_data.position[i].y += m_data.speed[i] * dt;
	}
}

/*float ProjectileComponentMgr::getProjectileSpeed()
{
	return m_projectileSpeed;
}*/
