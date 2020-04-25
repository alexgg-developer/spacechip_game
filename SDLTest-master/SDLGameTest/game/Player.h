#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include <unordered_map>

using namespace std;

class Player  
{
private :
	Entity m_entity;
	vec2 m_position;
	int32_t m_life = INITIAL_LIFE;
	float m_speed = INITIAL_SPEED;

public:
	static const int32_t SIZE; //pixels width and height
	static const int32_t INITIAL_LIFE;
	static const float INITIAL_SPEED;

	inline vec2 getPosition() { return m_position;  }	
	inline float getSpeed() { return m_speed; }
	inline int32_t getLife() { return m_life; }
	inline Entity getEntity() { return m_entity; }
	
	void reset();
	void setLife(int32_t life);
	void setPosition(const vec2& newPosition);
	void setEntity(const Entity& entity);
	
};

