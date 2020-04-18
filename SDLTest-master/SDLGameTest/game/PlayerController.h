#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include "Player.h"
#include "Input.h"
#include "Vec.h"

#include <unordered_map>

using namespace std;

class PlayerController  
{
private :
	Player& m_player;
	float m_leftLimit, m_rightLimit;
public:
	PlayerController(Player& player);
	void setLimits(float leftLimit, float rightLimit);
	void update(const Input& input, float dt);
};

