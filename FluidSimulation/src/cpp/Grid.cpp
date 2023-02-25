#include "Grid.h"


Grid::Grid
(
	std::vector <glm::vec3>& vertices,
	std::vector <unsigned int>& indices
)
{
	Grid::m_colors = new glm::vec4[N*N];
	Grid::m_instanceMatrix = new glm::mat4[N * N];

	Grid::vertices = vertices;
	Grid::indices = indices;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VertexBuffer VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	IndexBuffer EBO(indices);

	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
	
	// Create Instance Matrix Buffer
	glGenBuffers(1, &m_InstanceMT);
	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceMT);
	glBufferData(GL_ARRAY_BUFFER, N * N * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	// Can't link to a mat4 so you need to link four vec4s
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	// Makes it so the transform is only switched when drawing the next instance
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	// Create Color Buffer
	glGenBuffers(1, &m_ColorVB);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVB);
	glBufferData(GL_ARRAY_BUFFER, N * N * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
	// Link vec4 color
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	// Switch color
	glVertexAttribDivisor(5, 1);


	// Unbind all to prevent accidentally modifying them
	VAO.UnBind();
	VBO.UnBind();
	EBO.UnBind();
}


Grid::~Grid()
{
	delete[] m_colors;
	delete[] m_instanceMatrix;
}

void Grid::AddQuad(glm::vec2 pos, glm::vec4 color)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(pos, 0.0));
	m_instanceMatrix[IX((int)pos.x, (int)pos.y)] = model;
	m_colors[IX((int)pos.x, (int)pos.y)] = color;
}

void Grid::Update()
{
	// set color buffer to current
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVB);
	glBufferSubData(GL_ARRAY_BUFFER, 0, N * N * sizeof(glm::vec4), m_colors);

	// set instance matrix to current
	glBindBuffer(GL_ARRAY_BUFFER, m_InstanceMT);
	glBufferSubData(GL_ARRAY_BUFFER, 0, N * N * sizeof(glm::mat4), m_instanceMatrix);
}

void Grid::Draw(Shader& shader)
{
	// Bind shader to be able to access uniforms
	shader.Bind();
	VAO.Bind();

	glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instancing);
	
}