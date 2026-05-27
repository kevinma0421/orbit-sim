#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

#include "shader.h"
#include "utils.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    if (!vFile.is_open())
    {
        logMessage(vertexPath);
    }
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();
    std::string vCode = vStream.str();
    std::string fCode = fStream.str();
    const char *vShaderCode = vCode.c_str();
    const char *fShaderCode = fCode.c_str();

    // Compile shaders
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    // Link
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec4(const std::string &name, float r, float g, float b, float a) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), r, g, b, a);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
