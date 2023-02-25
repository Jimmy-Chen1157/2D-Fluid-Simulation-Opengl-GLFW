#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct ShaderProgramSource
{
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
public:
	unsigned int m_RendererId;
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();
	void Bind() const;
	void UnBind() const;

	void SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	void SetUniformMatrix4fv(const std::string& name, const glm::mat4& trans);

private:
	ShaderProgramSource ParseShader(const std::string& vertexPath, const std::string& fragmentPath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};