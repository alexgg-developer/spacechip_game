#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include <unordered_map>

using namespace std;

class Player  
{
private :
	vec3 m_position;
	int32_t m_life = INITIAL_LIFE;
	float m_speed = INITIAL_SPEED;

public:
	static const size_t SIZE; //pixels width and height
	static const int32_t INITIAL_LIFE;
	static const float INITIAL_SPEED;

	inline vec3 getPosition() { return m_position;  }	
	inline float getSpeed() { return m_speed; }
	inline int32_t getLife() { return m_life; }
	
	void reset();
	void setLife(int32_t life);
	void setPosition(const vec3& newPosition);
	
};

