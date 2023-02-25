#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	UnBind();
}

VertexBuffer::VertexBuffer(std::vector<glm::vec3>& positions)
{
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	UnBind();
}

VertexBuffer::VertexBuffer(std::vector<glm::mat4>& mat4s)
{
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
	UnBind();
}


VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

