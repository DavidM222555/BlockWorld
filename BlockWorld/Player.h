#pragma once

#include <glm/glm.hpp>

class Player
{
private:
	glm::vec3 _position;

public:
	inline glm::vec3 getPosition() { return _position;  }


	void updatePlayerPosition(int dx, int dy, int dz);
	void updatePlayerPosition(glm::vec3 newPosition);
};

