#pragma once

#include "Vec.h"

#include "../dodf/Instance.h"
#include "../dodf/Entity.h"

#include "Player.h"
#include "Input.h"
#include "Vec.h"

#include <unordered_map>
#include <functional>

using namespace std;

class PlayerController  
{
private :
	Player& m_player;
	float m_leftLimit, m_rightLimit;
	std::function <void(void)> m_shootCallback;
public:
	PlayerController(Player& player);
	void setLimits(float leftLimit, float rightLimit);
	void setShootCallback(std::function <void(void)> callback);
	void update(const Input& input, float dt);
};

