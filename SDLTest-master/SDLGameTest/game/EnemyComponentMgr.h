#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

//#include <glm/glm.hpp>

#include <unordered_map>

using namespace std;
//using namespace glm;using namespace std;

//class EnemyComponentMgr: public IComponentManager 
class EnemyComponentMgr  
{
private :
	struct InstanceData {
		size_t* life;
		//glm::vec3* position;
		vec3* position;
	};

	InstanceData m_data;
	size_t m_instanceCount;
	unordered_map<size_t, Instance> m_map;

public:
	static const size_t ENEMY_SIZE;
	//void add(const Entity& e) override;
	void add(const Entity& e, const vec3& position);
	vec3* const getPositions() { return m_data.position;  }
	Instance lookup(const Entity& e) const { return m_map.at(static_cast<size_t>(e.index())); }
	
	//void allocate(size_t size) override;
	void allocate(size_t size);
	void clean();
};

