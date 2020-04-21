#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "ProjectileComponentMgr.h"


using namespace dodf;


const size_t ProjectileComponentMgr::PROJECTILE_SIZE = 15;

void ProjectileComponentMgr::allocate(size_t size)
{
	m_data.position = static_cast<vec3*>(MemoryPool::Get(size * sizeof(vec3)));
	m_data.speed = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));
	m_data.textureID = static_cast<TextureMgr::TextureID*>(MemoryPool::Get(size * sizeof(TextureMgr::TextureID)));
}

void ProjectileComponentMgr::add(const Entity& e, const vec3 & position, float speed, TextureMgr::TextureID textureID)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.position[instance.i] = position;
	m_data.speed[instance.i] = speed;
	m_data.entity[instance.i] = e;
	m_data.textureID[instance.i] = textureID;
	++m_instanceCount;
}

void ProjectileComponentMgr::update(float dt)
{
	for (size_t i = 0; i < m_instanceCount; ++i) {
		m_data.position[i].y += m_data.speed[i] * dt;
	}
}

void ProjectileComponentMgr::checkOffLimits()
{
	for (ptrdiff_t i = m_instanceCount - 1; i >= 0; --i) {
		if (m_data.position[i].y > m_bottomLimit || m_data.position[i].y < m_topLimit) {
			m_destroyCallback(m_data.entity[i]);
			destroy(m_data.entity[i]);
		}
	}
}

void ProjectileComponentMgr::destroy(const Entity & e)
{
	Instance i = lookup(e);
	destroy(i);
}

void ProjectileComponentMgr::destroy(Instance i)
{
	ASSERT(i.i < m_instanceCount);
	size_t last = m_instanceCount - 1;
	m_map.erase(m_data.entity[i.i].index());

	Entity lastEntity = m_data.entity[last];
	m_data.entity[i.i] = m_data.entity[last];
	m_data.speed[i.i] = m_data.speed[last];
	m_data.position[i.i] = m_data.position[last];
	m_data.textureID[i.i] = m_data.textureID[last];

	m_map[lastEntity.index()] = i;

	--m_instanceCount;
}

void ProjectileComponentMgr::setDestroyCallback(std::function<void(Entity)> callback)
{
	m_destroyCallback = callback;
}

void ProjectileComponentMgr::setLimits(float bottomLimit, float topLimit)
{
	m_bottomLimit = bottomLimit;
	m_topLimit = topLimit;
}
