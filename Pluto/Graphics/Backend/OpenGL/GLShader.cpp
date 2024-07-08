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

using namespace pluto::Graphics;
GLShader::GLShader(GLShader::Properties *&&pProperties)
    : Shader(std::move(pProperties)), mLayout(new BufferLayout())
{
    if (!mProperties->filePath.empty())
    {
        // create from file
    }
    else
    {
        std::map<ShaderType, std::string> *sources = new std::map<ShaderType, std::string>();
        ReflectFromShaderData(mProperties->vertData, mProperties->vertDataSize, ShaderType::Vertex, *sources);
        ReflectFromShaderData(mProperties->fragData, mProperties->fragDataSize, ShaderType::Fragment, *sources);
        ReflectFromShaderData(mProperties->geomData, mProperties->geomDataSize, ShaderType::Geometry, *sources);
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
    delete mLayout;
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

void GLShader::ReflectFromShaderData(const uint32_t *data, uint32_t size, ShaderType type, std::map<ShaderType, std::string> &sources)
{
    std::vector<unsigned int> spv(data, data + size / sizeof(unsigned int));

    spirv_cross::CompilerGLSL *glsl = new spirv_cross::CompilerGLSL(std::move(spv));
    spirv_cross::ShaderResources resources = glsl->get_shader_resources();

    if (type == ShaderType::Vertex)
    {
        for (const spirv_cross::Resource &resource : resources.stage_inputs)
        {
            const spirv_cross::SPIRType &InputType = glsl->get_type(resource.type_id);
            const std::string &name = glsl->get_name(resource.id);
            // Switch to GL layout
            ReflectVertexInput(InputType, mLayout, name);
        }
    }

    // Get all sampled images in the shader.
    for (auto &resource : resources.sampled_images)
    {
        uint32_t set = glsl->get_decoration(resource.id, spv::DecorationDescriptorSet);
        uint32_t binding = glsl->get_decoration(resource.id, spv::DecorationBinding);
        auto &descriptorInfo = mDescriptorInfos[set];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.binding = binding;
        descriptor.name = resource.name;
        descriptor.shaderType = type;
        descriptor.type = Graphics::DescriptorType::ImageSampler;
    }
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

void ReflectVertexInput(const spirv_cross::SPIRType type, BufferLayout *layout, std::string name)
{
    switch (type.basetype)
    {
    case spirv_cross::SPIRType::Float:
        switch (type.vecsize)
        {
        case 1:
            layout->Push<float>(name);
            break;
        case 2:
            layout->Push<glm::vec2>(name);
            break;
        case 3:
            layout->Push<glm::vec3>(name);
            break;
        case 4:
            layout->Push<glm::vec4>(name);
            break;
        }
    case spirv_cross::SPIRType::Double:
        break;
    default:
        break;
    }
}