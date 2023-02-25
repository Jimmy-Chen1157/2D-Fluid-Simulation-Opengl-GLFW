#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererId);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererId);
}

void VertexArray::LinkAttrib(VertexBuffer& VBO, unsigned int layout, unsigned int numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.UnBind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererId);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}