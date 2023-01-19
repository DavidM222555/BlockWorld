#pragma once

#include "VertexBuffer.h"
#include "BufferLayout.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexArray
{
private:
	unsigned int _ID;

public:
	VertexArray();
	~VertexArray();

	void bind();
	void addBuffer(VertexBuffer& vb, BufferLayout& layout);

	unsigned int getID() { return _ID;  }
};

