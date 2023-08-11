#include "BufferLayout.h"

// Utilized TheCherno's following videos for some help on how to layout various OpenGL primitives:
// https://www.youtube.com/watch?v=x0H--CL2tUI&ab_channel=TheCherno

void BufferLayout::pushBufferLayout(unsigned int count, unsigned int type, bool normalized)
{
	unsigned int size = 0;

	switch (type) {
		case GL_FLOAT:
			size = 4;
			break;
		case GL_UNSIGNED_BYTE:
			size = 4;
			break;
		case GL_BYTE:
			size = 1;
			break;
		case GL_SHORT:
			size = 2;
			break;
	}

	_stride += size * count;

	elements.push_back({ size, type, count, false });
}
