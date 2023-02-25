#include "Shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    : m_RendererId(0)
{
    ShaderProgramSource source = ParseShader(vertexShaderPath, fragmentShaderPath);
    m_RendererId = CreateShader(source.vertexShader, source.fragmentShader);
}


ShaderProgramSource Shader::ParseShader(const std::string& vertexPath, const std::string& fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();		
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();		
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    return { vertexCode, fragmentCode };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shaderProgram;
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    unsigned int location = glGetUniformLocation(m_RendererId, name.c_str());
    if (location == -1) {
        std::cout << "Warning : Uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

void Shader::SetUniform4f(const std::string& name, float f1, float f2, float f3, float f4)
{
    glUniform4f(GetUniformLocation(name), f1, f2, f3, f4);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& trans)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(trans));
}


Shader::~Shader()
{
	glDeleteProgram(m_RendererId);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererId);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

