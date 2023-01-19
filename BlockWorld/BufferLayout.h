#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexBuffer.h"

#include <vector>
#include <iostream>

using std::vector;

struct BufferElement {
	unsigned int size;
	unsigned int type;
	unsigned int count;
	bool normalized;
};

class BufferLayout
{
private:
	vector<BufferElement> elements;
	unsigned int _stride;

public:
	inline vector<BufferElement>& getElements() { return elements;  }
	inline unsigned int getStride() { return _stride;  }
	void pushBufferLayout(unsigned int count, unsigned int type, bool normalized);

};

