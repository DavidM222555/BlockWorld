#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"

enum class Texture : char
{
	RED='r',
	GREEN='g',
	BLUE='b',
};


class Cube
{
private:
	bool _active;
	Texture _texture;

public:
	Cube();
	Cube(bool isActive);
	Cube(bool isActive, Texture texture);

	bool getActive() { return _active;  }
	void setActive(bool active);

	Texture getTexture() { return _texture;  }
	void setTexture(Texture texture) { _texture = texture; }
};

