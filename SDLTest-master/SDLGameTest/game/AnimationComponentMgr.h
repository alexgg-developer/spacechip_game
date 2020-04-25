#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"
#include "../dodf/Assert.h"

#include "TextureMgr.h"

#include <unordered_map>
#include <functional>

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SDL.h>
#elif defined(__APPLE__)
#include <SDL2/SDL.h>
#endif

using namespace std;

class AnimationComponentMgr  
{

public:
	enum AnimationType : uint32_t {
		LOOP,
		ONE_SHOT
	};

	inline TextureMgr::AnimationID* getAnimationID() const { return m_data.animationID; }
	inline size_t* getFrames() const { return m_data.frames; }
	inline size_t* getCurrentFrames() const { return m_data.currentFrame; }
	inline vec2 getPosition(const Entity& e) const { Instance i = lookup(e); return vec2{ m_data.x[i.i], m_data.y[i.i] }; }
	inline size_t const getSize() const { return m_instanceCount;  }
	inline float* const getXCoords() const { return m_data.x;  }
	inline float* const getYCoords() const { return m_data.y;  }
	inline Instance lookup(const Entity& e) const { return m_map.at(e.index()); }
	
	void add(const Entity& e,
		const vec2& position,
		float timePerFrame,
		size_t frames,
		AnimationType type,
		TextureMgr::AnimationID animationID
	);
	void allocate(size_t size);
	void destroy(Instance i);
	void destroy(const Entity& e);
	void reset();
	void setPosition(const Entity & e, const vec2 & position);
	void update(float dt);

private:

	struct InstanceData {
		TextureMgr::AnimationID* animationID;
		Entity* entity;
		float* x;
		float* y;
		float* timePerFrame;
		float* elapsedTime;
		size_t* frames;
		size_t* currentFrame;
		AnimationType* type;
	};

	InstanceData m_data;
	size_t m_instanceCount;
	unordered_map<uint32_t, Instance> m_map;
	size_t m_capacity;
};

