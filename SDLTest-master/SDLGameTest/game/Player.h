#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

//#include <glm/glm.hpp>

#include <unordered_map>

using namespace std;
//using namespace glm;using namespace std;

//class Player: public IComponentManager 
class Player  
{
private :
	size_t m_life = 3;
	vec3 m_position;
	float m_speed = 160.0f;

public:
	static const size_t SIZE; //pixels width and height
	vec3 getPosition() { return m_position;  }	
	float getSpeed() { return m_speed; }
	void setPosition(const vec3& newPosition);
	
};

