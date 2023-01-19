#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size) 
{
	glGenBuffers(1, &_ID);
	glBindBuffer(GL_ARRAY_BUFFER, _ID);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _ID);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}