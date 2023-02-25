#pragma once

#include <iostream>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define IX(x, y) ((x) + (y) * N)

extern const int N;
extern const int iter;


class Grid
{
private:
	unsigned int m_ColorVB;
	unsigned int m_InstanceMT;
	glm::vec4* m_colors;
	glm::mat4* m_instanceMatrix;

public:
	std::vector <glm::vec3> vertices;
	std::vector <unsigned int> indices;
	
	VertexArray VAO;

	unsigned int instancing = N*N;

	// Initializes the mesh
	Grid
	(
		std::vector <glm::vec3>& vertices,
		std::vector <unsigned int>& indices
	);
	~Grid();

	void AddQuad(glm::vec2 pos, glm::vec4 color);
	void Update();

	// Draws the mesh
	void Draw(Shader& shader);
};