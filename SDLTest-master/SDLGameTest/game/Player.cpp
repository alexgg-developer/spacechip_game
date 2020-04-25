#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "Player.h"


using namespace dodf;

const int32_t Player::SIZE = 30u;
const int32_t Player::INITIAL_LIFE = 3;
const float Player::INITIAL_SPEED = 160.0f;

void Player::reset()
{
	m_life = INITIAL_LIFE;
	m_speed = INITIAL_SPEED;
}

void Player::setLife(int32_t life)
{
	m_life = life;
}

void Player::setPosition(const vec2& newPosition)
{
	m_position = newPosition;
}

void Player::setEntity(const Entity & entity)
{
	m_entity = entity;
}
