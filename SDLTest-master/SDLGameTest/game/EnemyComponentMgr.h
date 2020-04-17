#pragma once

#include "../dodf/IComponentManager.h"
#include <glm/glm.hpp>

#include <unordered_map>

using namespace dodf;
using namespace std;
using namespace glm;

//class EnemyComponentMgr: public IComponentManager 
class EnemyComponentMgr  
{
private :
	struct InstanceData {
		size_t* life;
		glm::vec3* position;
	};

	InstanceData m_data;
	size_t m_instanceCount;

public:
	static const float ENEMY_SIZE;
	//void add(const Entity& e) override;
	void add(const Entity& e, const glm::vec3& position);
	glm::vec3* const getPositions() { return m_data.position;  }
	
	//void allocate(size_t size) override;
	void allocate(size_t size);
	void clean();
};

