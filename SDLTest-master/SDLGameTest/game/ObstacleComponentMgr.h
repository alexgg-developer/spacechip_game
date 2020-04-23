#pragma once

#include "Vec.h"

#include "../dodf/Assert.h"
#include "../dodf/Instance.h"
#include "../dodf/Entity.h"


#include <unordered_map>
#include <functional>

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#endif

using namespace std;

class ObstacleComponentMgr  
{
private :
	struct InstanceData {
		int32_t* life;
		Entity* entity;
		vec3* position;
	};

	InstanceData m_data;
	size_t m_instanceCount, m_capacity;
	unordered_map<uint32_t, Instance> m_map;
	std::function <void(Entity)> m_destroyCallback;

public:
	static const size_t OBSTACLE_SIZE;
	
	inline int32_t getLife(Instance i) { ASSERT(i.i < m_instanceCount); return m_data.life[i.i]; }
	inline vec3 getPosition(const Entity& e) const { return m_data.position[lookup(e).i];  }
	inline vec3* const getPositions() const { return m_data.position;  }
	inline size_t const getSize() const { return m_instanceCount;  }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }
	
	void add(const Entity& e, const vec3& position);
	void allocate(size_t size);
	Entity checkShot(const SDL_Rect& rect);
	void destroy(const Entity& e);
	void reset();
	void setDestroyCallback(std::function <void(Entity)> callback);
	void setLife(Instance i, int32_t life);
};

