#include "../dodf/MemoryPool.h"
#include "../dodf/Assert.h"

#include "Player.h"


using namespace dodf;

const size_t Player::SIZE = 30;

void Player::setPosition(const vec3& newPosition)
{
	m_position = newPosition;
}
