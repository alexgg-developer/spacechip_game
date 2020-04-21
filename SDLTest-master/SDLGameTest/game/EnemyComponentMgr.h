#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"
#include "../dodf/Assert.h"

//#include <glm/glm.hpp>

#include <unordered_map>
#include <functional>

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#endif

using namespace std;
//using namespace glm;using namespace std;

//class EnemyComponentMgr: public IComponentManager 
class EnemyComponentMgr  
{
private :
	struct InstanceData {
		size_t* life;
		Entity* entity;
		vec3* position;
	};
	
	float m_speed, m_leftLimit, m_rightLimit;
	InstanceData m_data;
	size_t m_instanceCount;
	unordered_map<uint32_t, Instance> m_map;
	std::function <void(void)> m_limitReached;

public:
	static const size_t ENEMY_SIZE;

	inline int32_t getLife(Instance i) { ASSERT(i.i < m_instanceCount); return m_data.life[i.i]; }
	inline vec3 getPosition(const Entity& e) const { return m_data.position[lookup(e).i];  }
	inline vec3* const getPositions() const { return m_data.position;  }
	inline size_t const getSize() const { return m_instanceCount;  }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }
	
	void add(const Entity& e, const vec3& position);
	//void allocate(size_t size) override;
	void allocate(size_t size);
	void checkLimits();
	Entity checkShot(const SDL_Rect& rect);
	void clean();
	void destroy(const Entity& e);
	void setLife(Instance i, int32_t life);
	void setLimits(float leftLimit, float rightLimit);
	void setLimitReachedCallback(std::function <void(void)> callback);
	void setSpeed(float speed);
	void update(float dt);
};

