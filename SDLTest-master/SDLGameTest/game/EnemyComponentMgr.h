#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"
#include "../dodf/Assert.h"

#include <unordered_map>
#include <functional>

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#endif

using namespace std;

class EnemyComponentMgr  
{
private :
	struct InstanceData {
		int32_t* life;
		Entity* entity;
		float* x;
		float* y;
	};
	
	float m_speed, m_leftLimit, m_rightLimit;
	InstanceData m_data;
	size_t m_instanceCount;
	unordered_map<uint32_t, Instance> m_map;
	size_t m_capacity;

public:
	static const int32_t ENEMY_SIZE;

	inline int32_t getLife(Instance i) { ASSERT(i.i < m_instanceCount); return m_data.life[i.i]; }
	inline vec2 getPosition(const Entity& e) const { Instance i = lookup(e); return vec2{ m_data.x[i.i], m_data.y[i.i] }; }
	inline vec2 getPosition(Instance i) const { ASSERT(i.i < m_instanceCount); return vec2{ m_data.x[i.i], m_data.y[i.i] }; }
	inline float* const getXCoords() const { return m_data.x;  }
	inline float* const getYCoords() const { return m_data.y;  }
	inline size_t const getSize() const { return m_instanceCount;  }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }
	
	void add(const Entity& e, const vec2& position);
	void allocate(size_t size);
	void checkLimits();
	Entity checkShot(const SDL_Rect& rect);
	void destroy(const Entity& e);
	void reset();
	void setLife(Instance i, int32_t life);
	void setLimits(float leftLimit, float rightLimit);
	void setSpeed(float speed);
	void update(float dt);
};

