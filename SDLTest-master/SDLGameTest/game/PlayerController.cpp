#include "PlayerController.h"

#include <cmath>

PlayerController::PlayerController(Player & player) : m_player(player), m_rightLimit(0), m_leftLimit(0)
{
}

void PlayerController::setLimits(float leftLimit, float rightLimit)
{
	m_leftLimit = leftLimit;
	m_rightLimit = rightLimit;
}

void PlayerController::setShootCallback(std::function <void(void)> callback)
{
	m_shootCallback = callback;
}



void PlayerController::update(const Input& input, float dt)
{
	auto position = m_player.getPosition();
	float speed = m_player.getSpeed();

	if (input.check(Input::KLEFT)) {
		position.x -= speed * dt;
	}

	if (input.check(Input::KRIGHT)) {
		position.x += speed * dt;
	}

	position.x = fmax(position.x, m_leftLimit);
	position.x = fmin(position.x, m_rightLimit);
	m_player.setPosition(position);


	if (input.checkPressed(Input::KSPACE)) {
		m_shootCallback();
	}
}
