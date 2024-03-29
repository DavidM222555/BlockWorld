#include "VertexArray.h"

// Utilized TheCherno's following videos for some help on how to layout various OpenGL primitives:
// https://www.youtube.com/watch?v=x0H--CL2tUI&ab_channel=TheCherno

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_ID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_ID);
}

void VertexArray::bind()
{
	glBindVertexArray(_ID);
}

void VertexArray::addBuffer(VertexBuffer& vb, BufferLayout& layout) 
{
	bind();
	vb.bind();

	const auto& elements = layout.getElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, GL_FALSE, 
			layout.getStride(), (const void*)offset);

		offset += element.count * element.size;
	}
}
