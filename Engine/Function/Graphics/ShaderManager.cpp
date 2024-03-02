#include "Engine/Function/Graphics/ShaderManager.h"
#include <iostream>
using namespace NoobRenderer;

ShaderManager::ShaderManager(token)
{
}
void ShaderManager::Init()
{
}
ShaderManager::~ShaderManager()
{
    for (auto &_shader : m_shaders)
    {
        _shader.reset();
    }
    m_shaders.clear();
}
ShaderIndex ShaderManager::LoadShader(std::string_view vShaderName, std::string_view fShaderName)
{
    auto concat_str = std::string(vShaderName) + std::string(fShaderName);
    if (m_indices.find(concat_str) != m_indices.end())
    {
        std::cout << "Shader:" << concat_str << " has been Loaded, return the loaded version." << std::endl;
        return m_indices[concat_str];
    }

    ShaderProgram::Ptr newShaderProgram = std::make_shared<ShaderProgram>();
    newShaderProgram->SetVertexShader(vShaderName.data());
    newShaderProgram->SetFragmentShader(fShaderName.data());
    newShaderProgram->BuildShaderProgram();

    auto idx = newShaderID;
    m_indices[concat_str] = newShaderID++;
    m_shaders.emplace_back(newShaderProgram);
    return idx;
}
ShaderProgram::Ptr ShaderManager::LoadShaderAndGet(std::string_view vShaderName, std::string_view fShaderName)
{
    return m_shaders[LoadShader(vShaderName, fShaderName)];
}
ShaderIndex ShaderManager::LoadShader(std::string_view vShaderName, std::string_view fShaderName, std::string_view gShaderName, bool duplicate)
{
    auto concat_str = std::string(vShaderName) + std::string(fShaderName) + std::string(gShaderName);
    if (m_indices.find(concat_str) != m_indices.end() && !duplicate)
    {
        std::cout << "Shader:" << concat_str << " has been Loaded, return the loaded version." << std::endl;
        return m_indices[concat_str];
    }

    ShaderProgram::Ptr newShaderProgram = std::make_shared<ShaderProgram>();
    newShaderProgram->SetVertexShader(vShaderName.data());
    newShaderProgram->SetFragmentShader(fShaderName.data());
    newShaderProgram->SetGeometryShader(gShaderName.data());
    newShaderProgram->BuildShaderProgram(true);

    if (m_indices.find(concat_str) != m_indices.end() && duplicate)
    {
        std::cout << "Shader:" << concat_str << " has been Loaded, but load a new copy." << std::endl;
        concat_str += std::to_string(newShaderProgram->m_programID);
    }

    auto idx = newShaderID;
    m_indices[concat_str] = newShaderID++;
    m_shaders.emplace_back(newShaderProgram);
    return idx;
}
ShaderProgram::Ptr ShaderManager::LoadShaderAndGet(std::string_view vShaderName, std::string_view fShaderName, std::string_view gShaderName, bool duplicate)
{
    return m_shaders[LoadShader(vShaderName, fShaderName, gShaderName, duplicate)];
}
GLuint ShaderManager::GetShaderGLProgram(unsigned int index)
{
    return m_shaders[index]->GetShaderProgram();
}
ShaderProgram::Ptr ShaderManager::GetShaderProgram(unsigned int index)
{
    return m_shaders[index];
}
void ShaderManager::RebuildShaders()
{
    for (auto &_shader : m_shaders)
        _shader->BuildShaderProgram();
}
