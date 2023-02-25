#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(std::vector<unsigned int>& indices)
{
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
