#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include <unordered_map>

using namespace std;

class ProjectileComponentMgr  
{
private :
	/*struct EnemyProjectileData {
		vec3* position;
	};
	EnemyProjectileData m_enemyProjectiledata;
	size_t m_enemyProjectileCount;

	struct PlayerProjectileData {
		vec3* position;
	};
	PlayerProjectileData m_playerProjectileData;
	size_t m_playerProjectileCount;*/

	struct InstanceData {
		vec3* position;
		float* speed ;
	};
	InstanceData m_data;
	size_t m_instanceCount;

	unordered_map<size_t, Instance> m_map;
public:
	static const size_t PROJECTILE_SIZE;
	//void allocate(size_t playerProjectiles, size_t enemyProjectiles)
	void allocate(size_t size);
	void add(const Entity& e, const vec3 & position, float speed);
	vec3* const getPositions() { return m_data.position; }
	std::pair<vec3*, size_t> const getPositionsAndSize() { return std::make_pair(m_data.position, m_instanceCount); }
	size_t const getSize() { return m_instanceCount; }


	void update(float dt);
	
};

