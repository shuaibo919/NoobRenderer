/* OpenGL Shader */
#include "Graphics/Backend/OpenGL/GLShader.h"
/* Usage */
#include "Core/FileSystem.h"
#include "Core/Utilities.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"
/* Third */
#include "spirv_cross.hpp"
#include "spirv_glsl.hpp"
#include "nlohmann/json.hpp"

using ShaderJson = nlohmann::json;
using namespace pluto::Graphics;

uint32_t CompileAll(std::map<ShaderType, std::pair<std::string, std::string>> &sources, OpenGL::ShaderErrorInfo &info);

GLShader::GLShader(GLShader::Properties *&&pProperties)
    : Shader(std::move(pProperties))
{
    if (!mProperties->filePath.empty())
    {
        std::string realpath = FileSystem::Instance().GetPhysicalPath(mProperties->filePath);
        ShaderJson shaderDesc = ShaderJson::parse(FileSystem::Instance().ReadFile(realpath));
        std::map<ShaderType, std::pair<std::string, std::string>> sources;
        for (auto &shader : shaderDesc)
        {
            auto type = shader["type"].get<ShaderType>();
            sources.insert({type, {shader["name"].get<std::string>(), shader["glsl"].get<std::string>()}});
            mProperties->types.push_back(type);
        }

        OpenGL::ShaderErrorInfo error;
        mHandle = CompileAll(sources, error);
    }
    else
    {
        log<Error>("OpenGL Shader: No file path provided");
    }
}

GLShader::~GLShader()
{
}

bool GLShader::IsCompiled()
{
    return false;
}

uint32_t CompileShader(ShaderType type, std::pair<std::string, std::string> &source, uint32_t program, OpenGL::ShaderErrorInfo &info)
{
    const char *cstr = source.second.c_str();

    GlCall(GLuint shader = glCreateShader(GLUtilities::GetShaderType(type)));
    GlCall(glShaderSource(shader, 1, &cstr, NULL));
    GlCall(glCompileShader(shader));

    GLint result;
    GlCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        pluto::log<pluto::Error>("OpenGL Shader [%s]: Compilation failed", source.first.c_str());
        return 0;
    }
    return shader;
}

uint32_t CompileAll(std::map<ShaderType, std::pair<std::string, std::string>> &sources, OpenGL::ShaderErrorInfo &info)
{
    uint32_t program = glCreateProgram();
    std::vector<GLuint> shaders;
    std::string glVersion;

    for (auto source : sources)
    {
        shaders.push_back(CompileShader(source.first, source.second, program, info));
    }

    for (unsigned int shader : shaders)
        glAttachShader(program, shader);

    GlCall(glLinkProgram(program));

    GLint result;
    GlCall(glGetProgramiv(program, GL_LINK_STATUS, &result));
    if (result == GL_FALSE)
    {
        pluto::log<pluto::Error>("OpenGL Shader: Link failed");
        return 0;
    }

    GlCall(glValidateProgram(program));

    for (int z = 0; z < shaders.size(); z++)
        glDetachShader(program, shaders[z]);

    for (int z = 0; z < shaders.size(); z++)
        glDeleteShader(shaders[z]);

    return program;
}