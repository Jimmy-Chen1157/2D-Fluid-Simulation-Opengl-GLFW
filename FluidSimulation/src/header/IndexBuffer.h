#pragma once

#include <vector>
#include <glad/glad.h>

class IndexBuffer	
{
private:
	unsigned int m_RendererId;

public:
	IndexBuffer(std::vector<unsigned int>& indices);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

};