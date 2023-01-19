#include "Cube.h"

Cube::Cube()
{
	_texture = Texture::RED;
	_active = false;
}


Cube::Cube(bool isActive)
{
	_texture = Texture::RED;
	_active = isActive;
}

Cube::Cube(bool isActive, Texture texture)
{
	_texture = texture;
	_active = isActive;
}

void Cube::setActive(bool active)
{
	_active = active;
}

