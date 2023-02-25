#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class VertexBuffer 
{
private:
	unsigned int m_RendererId;
public:
	VertexBuffer(std::vector<Vertex>&vertices);
	VertexBuffer(std::vector<glm::vec3>& positions);
	VertexBuffer(std::vector<glm::mat4>& mat4s);
	~VertexBuffer();
	void Bind() const;
	void UnBind() const;
};