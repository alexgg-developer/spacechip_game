#pragma once

#include "IComponentManager.h"
#include <glm/glm.hpp>

#include <unordered_map>

using namespace dodf;
using namespace std;
using namespace glm;

class EnemyComponentMgr: public IComponentManager 
{
private :
	struct InstanceData {
		size_t* life;
	};

	InstanceData m_data;

public:
	
	void add(const Entity& e) override;
	
	void allocate(size_t size) override;
	void clean();
};

