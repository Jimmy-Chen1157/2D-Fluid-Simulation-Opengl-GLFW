#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"


class VertexArray
{
private:
	unsigned int m_RendererId;
public:

	VertexArray();
	~VertexArray();

	void LinkAttrib(VertexBuffer& VBO, unsigned int layout, unsigned int numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind() const;
	void UnBind() const;
};