#include "Engine/Function/Graphics/Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace NoobRenderer;

ShaderProgram::ShaderProgram()
{
    m_programID = glCreateProgram();
    m_use_geometry_shader = false;
}
ShaderProgram ::~ShaderProgram()
{
    if (m_programID != 0)
    {
        if (vShaderID != 0)
        {
            glDetachShader(m_programID, vShaderID);
            glDeleteShader(vShaderID);
        }
        if (gShaderID != 0)
        {
            glDetachShader(m_programID, gShaderID);
            glDeleteShader(gShaderID);
        }
        if (fShaderID != 0)
        {
            glDetachShader(m_programID, fShaderID);
            glDeleteShader(fShaderID);
        }
        glDeleteProgram(m_programID);
    }
}
bool ShaderProgram::CompileShader(GLuint &programID, GLuint &shaderID, const std::string &shaderSrc, GLenum type)
{
    std::string glslCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(shaderSrc);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        glslCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ShaderProgram::File Not Succesfully read! Shader Path:" << shaderSrc << std::endl;
    }
    const char *shaderCode = glslCode.c_str();
    shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ShaderProgram::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return false;
    }
    glAttachShader(programID, shaderID);
    glLinkProgram(programID);
    glDeleteShader(shaderID);
    return true;
}
void ShaderProgram::SetVertexShader(const std::string &name)
{
    vShaderName = name;
}
std::string ShaderProgram::GetVertexShaderName()
{
    return vShaderName;
}

void ShaderProgram::SetFragmentShader(const std::string &name)
{
    fShaderName = name;
}
std::string ShaderProgram::GetFragmentShaderName()
{
    return fShaderName;
}

void ShaderProgram::SetGeometryShader(const std::string &name)
{
    gShaderName = name;
}
std::string ShaderProgram::GetGeometryShaderName()
{
    return gShaderName;
}

void ShaderProgram::BuildShaderProgram(bool use_geometry_shader)
{
    m_use_geometry_shader = use_geometry_shader;
    CompileShader(m_programID, vShaderID, vShaderName, GL_VERTEX_SHADER);
    if (use_geometry_shader)
        CompileShader(m_programID, gShaderID, gShaderName, GL_GEOMETRY_SHADER);
    CompileShader(m_programID, fShaderID, fShaderName, GL_FRAGMENT_SHADER);
}
GLuint ShaderProgram::GetShaderProgram()
{
    return m_programID;
}
void ShaderProgram::SetUniform(const GLchar *name, bool value)
{
    glUseProgram(m_programID);
    glUniform1i(glGetUniformLocation(m_programID, name), static_cast<int>(value));
}
void ShaderProgram::SetUniform(const GLchar *name, int value)
{
    glUseProgram(m_programID);
    glUniform1i(glGetUniformLocation(m_programID, name), value);
}
void ShaderProgram::SetUniform(const GLchar *name, unsigned int value)
{
    glUseProgram(m_programID);
    glUniform1ui(glGetUniformLocation(m_programID, name), value);
}
void ShaderProgram::SetUniform(const GLchar *name, float value)
{
    glUseProgram(m_programID);
    glUniform1f(glGetUniformLocation(m_programID, name), value);
}
void ShaderProgram::SetUniform(const GLchar *name, glm::vec2 vec2)
{
    glUseProgram(m_programID);
    glUniform2fv(glGetUniformLocation(m_programID, name), 1, glm::value_ptr(vec2));
}
void ShaderProgram::SetUniform(const GLchar *name, glm::vec3 vec3)
{
    glUseProgram(m_programID);
    glUniform3fv(glGetUniformLocation(m_programID, name), 1, glm::value_ptr(vec3));
}
void ShaderProgram::SetUniform(const GLchar *name, glm::vec4 vec4)
{
    glUseProgram(m_programID);
    glUniform4fv(glGetUniformLocation(m_programID, name), 1, glm::value_ptr(vec4));
}
void ShaderProgram::SetUniform(const GLchar *name, glm::mat3 mat3)
{
    glUseProgram(m_programID);
    glUniformMatrix3fv(glGetUniformLocation(m_programID, name), 1, GL_FALSE, glm::value_ptr(mat3));
}
void ShaderProgram::SetUniform(const GLchar *name, glm::mat4 mat4)
{
    glUseProgram(m_programID);
    glUniformMatrix4fv(glGetUniformLocation(m_programID, name), 1, GL_FALSE, glm::value_ptr(mat4));
}
