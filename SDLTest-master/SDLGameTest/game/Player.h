#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include <unordered_map>

using namespace std;

class Player  
{
private :
	int32_t m_life = 3;
	vec3 m_position;
	float m_speed = 160.0f;

public:
	static const size_t SIZE; //pixels width and height

	inline vec3 getPosition() { return m_position;  }	
	inline float getSpeed() { return m_speed; }
	inline int32_t getLife() { return m_life; }
	void setLife(int32_t life);

	void setPosition(const vec3& newPosition);
	
};

