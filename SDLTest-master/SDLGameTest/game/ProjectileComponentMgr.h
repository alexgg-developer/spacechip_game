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
		float* x;
		float* y;
		float* speed;
		Entity* entity;
		TextureMgr::TextureID* textureID;

	};
	InstanceData m_data;
	size_t m_instanceCount, m_capacity;
	unordered_map<uint32_t, Instance> m_map;

	float m_bottomLimit, m_topLimit;
	std::function <void(Entity)> m_destroyCallback;
public:
	static const size_t PROJECTILE_SIZE;
	//void allocate(size_t playerProjectiles, size_t enemyProjectiles)
	void allocate(size_t size);
	void add(const Entity& e, const vec2 & position, float speed, TextureMgr::TextureID textureID);
	inline vec2 getPosition(const Entity& e) const { Instance i = lookup(e); return vec2{ m_data.x[i.i], m_data.y[i.i] }; }
	inline float* const getXCoords() const { return m_data.x; }
	inline float* const getYCoords() const { return m_data.y; }
	inline TextureMgr::TextureID* const getTextureIDs() { return m_data.textureID; }
	inline size_t getSize() const { return m_instanceCount; }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }

	void checkOffLimits();
	void destroy(const Entity & e);
	void destroy(Instance i);
	void reset();
	void setDestroyCallback(std::function <void(Entity)> callback);
	void setLimits(float bottomLimit, float topLimit);
	void update(float dt);
	
};

