/* OpenGL Shader */
#include "Graphics/Backend/OpenGL/GLShader.h"
/* Usage */
#include "Core/FileSystem.h"
#include "Core/Utilities.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLShader::GLShader(GLShader::Properties *&&pProperties)
    : Shader(std::move(pProperties))
{
    if (!mProperties->filePath.empty())
    {
        // create from file
    }
    else
    {
        std::map<ShaderType, std::string> *sources = new std::map<ShaderType, std::string>();
        LoadFromData(mProperties->vertData, mProperties->vertDataSize, ShaderType::Vertex, *sources);
        LoadFromData(mProperties->fragData, mProperties->fragDataSize, ShaderType::Fragment, *sources);
        LoadFromData(mProperties->geomData, mProperties->geomDataSize, ShaderType::Geometry, *sources);
        // LoadFromData(mProperties->compData, mProperties->compDataSize, ShaderType::Compute, *sources);

        for (auto &source : *sources)
        {
            mProperties->types.push_back(source.first);
        }

        OpenGL::ShaderErrorInfo error;
        mHandle = CompileAll(sources, error);

        CreateLocations();

        delete sources;
    }
}

GLShader::~GLShader()
{
}

bool GLShader::CreateLocations()
{
    // todo
    return false;
}

bool GLShader::SetUniformLocation(const std::string &szName, bool pc)
{
    // todo
    return false;
}

bool GLShader::IsCompiled()
{
    return false;
}

void GLShader::BindPushConstants(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Pipeline> pipeline)
{
}

void GLShader::LoadFromData(const uint32_t *data, uint32_t size, ShaderType type, std::map<ShaderType, std::string> &sources)
{
}

uint32_t GLShader::CompileShader(ShaderType type, std::string source, uint32_t program, OpenGL::ShaderErrorInfo &info)
{
    const char *cstr = source.c_str();

    GlCall(GLuint shader = glCreateShader(GLUtilities::GetShaderType(type)));
    GlCall(glShaderSource(shader, 1, &cstr, NULL));
    GlCall(glCompileShader(shader));

    GLint result;
    GlCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        // todo
        return 0;
    }
    return shader;
}

uint32_t GLShader::CompileAll(std::map<ShaderType, std::string> *sources, OpenGL::ShaderErrorInfo &info)
{
    uint32_t program = glCreateProgram();
    std::vector<GLuint> shaders;
    std::string glVersion;

    for (auto source : *sources)
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
        // todo
        return 0;
    }

    GlCall(glValidateProgram(program));

    for (int z = 0; z < shaders.size(); z++)
        glDetachShader(program, shaders[z]);

    for (int z = 0; z < shaders.size(); z++)
        glDeleteShader(shaders[z]);

    return program;
}
