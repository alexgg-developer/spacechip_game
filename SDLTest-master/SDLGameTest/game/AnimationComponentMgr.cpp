#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "AnimationComponentMgr.h"

#include <cmath>

using namespace dodf;

void AnimationComponentMgr::destroy(Instance i)
{
	ASSERT(i.i < m_instanceCount);
	auto indexEntity = m_data.entity[i.i].index();
	size_t last = m_instanceCount - 1;	
	Entity lastEntity = m_data.entity[last];
	m_data.entity[i.i] = m_data.entity[last];
	m_data.x[i.i] = m_data.x[last];
	m_data.y[i.i] = m_data.y[last];
	m_data.timePerFrame[i.i] = m_data.timePerFrame[last];
	m_data.elapsedTime[i.i] = m_data.elapsedTime[last];
	m_data.frames[i.i] = m_data.frames[last];
	m_data.currentFrame[i.i] = m_data.currentFrame[last];
	m_data.type[i.i] = m_data.type[last];

	m_map[lastEntity.index()] = i;
	m_map.erase(indexEntity);
	--m_instanceCount;
}

void AnimationComponentMgr::destroy(const Entity & e)
{
	Instance i = lookup(e);
	destroy(i);
}

void AnimationComponentMgr::reset()
{
	memset(m_data.animationID, 0, m_capacity * sizeof(TextureMgr::AnimationID));
	memset(m_data.entity, 0, m_capacity * sizeof(Entity));
	memset(m_data.x, 0, m_capacity * sizeof(float));
	memset(m_data.y, 0, m_capacity * sizeof(float));
	memset(m_data.timePerFrame, 0, m_capacity * sizeof(float));
	memset(m_data.elapsedTime, 0, m_capacity * sizeof(float));
	memset(m_data.frames, 0, m_capacity * sizeof(size_t));
	memset(m_data.currentFrame, 0, m_capacity * sizeof(size_t));
	memset(m_data.type, 0, m_capacity * sizeof(AnimationType));
	
	m_map.clear();
	m_instanceCount = 0;
}

void AnimationComponentMgr::setPosition(const Entity & e, const vec2 & position)
{
	Instance i = lookup(e);
	m_data.x[i.i] = position.x;
	m_data.y[i.i] = position.y;
}


void AnimationComponentMgr::allocate(size_t size)
{
	m_data.animationID = static_cast<TextureMgr::AnimationID*>(MemoryPool::Get(size * sizeof(TextureMgr::AnimationID)));
	m_data.entity = static_cast<Entity*>(MemoryPool::Get(size * sizeof(Entity)));
	m_data.x = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.y = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.timePerFrame = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.elapsedTime = static_cast<float*>(MemoryPool::Get(size * sizeof(float)));
	m_data.frames = static_cast<size_t*>(MemoryPool::Get(size * sizeof(size_t)));
	m_data.currentFrame = static_cast<size_t*>(MemoryPool::Get(size * sizeof(size_t)));
	m_data.type = static_cast<AnimationType*>(MemoryPool::Get(size * sizeof(AnimationType)));
	
	m_instanceCount = 0;
	m_capacity = size;
}

void AnimationComponentMgr::add(const Entity& e, 
	const vec2& position, 
	float timePerFrame, 
	size_t frames, 
	AnimationType type,
	TextureMgr::AnimationID animationID
	)
{
	auto instance = Instance::create(m_instanceCount);
	m_map[e.index()] = instance;
	m_data.entity[instance.i] = e;
	m_data.animationID[instance.i] = animationID;
	m_data.x[instance.i] = position.x;
	m_data.y[instance.i] = position.y;
	m_data.timePerFrame[instance.i] = timePerFrame;
	m_data.elapsedTime[instance.i] = 0.0f;
	m_data.frames[instance.i] = frames;
	m_data.currentFrame[instance.i] = 0u;
	m_data.type[instance.i] = type;

	++m_instanceCount;
}

void AnimationComponentMgr::update(float dt)
{
	for (ptrdiff_t i = m_instanceCount - 1; i >= 0; --i) {
		m_data.elapsedTime[i] += dt;
		if (m_data.elapsedTime[i] > m_data.timePerFrame[i]) {
			m_data.elapsedTime[i] -= m_data.timePerFrame[i];
			++m_data.currentFrame[i];
			if (m_data.currentFrame[i] == m_data.frames[i]) {
				m_data.currentFrame[i] = 0;
				if (m_data.type[i] == AnimationType::ONE_SHOT) {
					destroy(Instance::create(i));
				}
			}
		}
	}
}




