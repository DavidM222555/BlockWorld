#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexBuffer
{
private:
	unsigned int _ID;

public:
	VertexBuffer(const void* data, size_t size);

	void bind();
	void unbind();

};

