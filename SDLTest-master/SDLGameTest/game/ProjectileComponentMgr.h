#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include "TextureMgr.h"

#include <unordered_map>
#include <functional>

using namespace std;

class ProjectileComponentMgr  
{
private :

	struct InstanceData {
		vec3* position;
		float* speed;
		Entity* entity;
		TextureMgr::TextureID* textureID;

	};
	InstanceData m_data;
	size_t m_instanceCount;
	unordered_map<uint32_t, Instance> m_map;

	float m_bottomLimit, m_topLimit;
	std::function <void(Entity)> m_destroyCallback;
public:
	static const size_t PROJECTILE_SIZE;
	//void allocate(size_t playerProjectiles, size_t enemyProjectiles)
	void allocate(size_t size);
	void add(const Entity& e, const vec3 & position, float speed, TextureMgr::TextureID textureID);
	inline vec3 getPosition(const Entity& e) { return m_data.position[lookup(e).i]; }
	inline vec3* const getPositions() { return m_data.position; }
	inline TextureMgr::TextureID* const getTextureIDs() { return m_data.textureID; }
	inline std::pair<vec3*, size_t> const getPositionsAndSize() { return std::make_pair(m_data.position, m_instanceCount); }
	inline size_t const getSize() { return m_instanceCount; }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }

	void checkOffLimits();
	void destroy(const Entity & e);
	void destroy(Instance i);
	void setDestroyCallback(std::function <void(Entity)> callback);
	void setLimits(float bottomLimit, float topLimit);
	void update(float dt);
	
};

