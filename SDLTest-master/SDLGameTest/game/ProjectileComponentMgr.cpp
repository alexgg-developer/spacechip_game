#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "ProjectileComponentMgr.h"


using namespace dodf;


const size_t ProjectileComponentMgr::PROJECTILE_SIZE = 15u;

void ProjectileComponentMgr::allocate(size_t size)
{
	m_data.x = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.y = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.speed = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));
	m_data.textureID = static_cast<TextureMgr::TextureID*>(MemoryPool::Get(size * sizeof(TextureMgr::TextureID)));
	m_capacity = size;
}

void ProjectileComponentMgr::add(const Entity& e, const vec2 & position, float speed, TextureMgr::TextureID textureID)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.x[instance.i] = position.x;
	m_data.y[instance.i] = position.y;
	m_data.speed[instance.i] = speed;
	m_data.entity[instance.i] = e;
	m_data.textureID[instance.i] = textureID;
	++m_instanceCount;
}

void ProjectileComponentMgr::update(float dt)
{
	for (size_t i = 0; i < m_instanceCount; ++i) {
		m_data.y[i] += m_data.speed[i] * dt;
	}
}

void ProjectileComponentMgr::checkOffLimits()
{
	for (ptrdiff_t i = m_instanceCount - 1; i >= 0; --i) {
		if (m_data.y[i] > m_bottomLimit || m_data.y[i] < m_topLimit) {
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
	auto index = m_data.entity[i.i].index();

	Entity lastEntity = m_data.entity[last];
	m_data.entity[i.i] = m_data.entity[last];
	m_data.speed[i.i] = m_data.speed[last];
	m_data.x[i.i] = m_data.x[last];
	m_data.y[i.i] = m_data.y[last];
	m_data.textureID[i.i] = m_data.textureID[last];

	m_map[lastEntity.index()] = i;
	m_map.erase(index);

	--m_instanceCount;
}

void ProjectileComponentMgr::reset()
{
	memset(m_data.x, 0, m_capacity * sizeof(float));
	memset(m_data.y, 0, m_capacity * sizeof(float));
	memset(m_data.entity, 0, m_capacity * sizeof(Entity));
	memset(m_data.speed, 0, m_capacity * sizeof(float));
	memset(m_data.textureID, 0, m_capacity * sizeof(TextureMgr::TextureID));

	m_map.clear();
	m_instanceCount = 0;
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
