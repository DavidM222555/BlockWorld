#include "Player.h"

void Player::updatePlayerPosition(int dx, int dy, int dz)
{
	_position.x += dx;
	_position.y += dy;
	_position.z += dz;
}

void Player::updatePlayerPosition(glm::vec3 newPosition)
{
	_position = newPosition;
}
