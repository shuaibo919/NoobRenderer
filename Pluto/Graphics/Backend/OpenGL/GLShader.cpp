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

uint32_t CompileAll(std::map<ShaderType, std::pair<std::string, std::string>> &sources);

GLShader::GLShader(RenderContext *ctx, GLShader::Properties *&&pProperties)
    : Shader(ctx, std::move(pProperties))
{
    if (!mProperties->filePath.empty())
    {
        std::string realpath = FileSystem::Instance().GetPhysicalPath(mProperties->filePath);
        std::string jsonContent = FileSystem::Instance().ReadTextFile(realpath);
        ShaderJson shaderDesc = ShaderJson::parse(jsonContent);
        std::map<ShaderType, std::pair<std::string, std::string>> sources;
        for (auto &shader : shaderDesc)
        {
            auto type = shader["type"].get<ShaderType>();
            sources.insert({type, {shader["name"].get<std::string>(), shader["glsl"].get<std::string>()}});
            mProperties->types.push_back(type);
            ReadReflectInfo(shader["reflection"], type);
        }

        mHandle = CompileAll(sources);
        mCompiled = true;
    }
    else
    {
        log<Error>("OpenGL Shader: No file path provided");
    }
}

GLShader::~GLShader()
{
    glDeleteProgram(mHandle);
}

bool GLShader::IsCompiled()
{
    return mCompiled;
}

void GLShader::ReadReflectInfo(ShaderJson &info, ShaderType type)
{

    for (auto &vertexInput : info["VertexInput"])
    {
        auto inputType = static_cast<ShaderDataType>(vertexInput);
    }

    for (auto &resource : info["SampledImages"])
    {
        auto &descriptorInfo = mShaderReflectInfo[resource["set"].get<uint32_t>()];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.binding = resource["binding"].get<uint32_t>();
        descriptor.name = resource["name"].get<std::string>();
        descriptor.shaderType = static_cast<ShaderType>(resource["shaderType"].get<uint8_t>());
        descriptor.descType = DescriptorType::ImageSampler;
    }

    for (auto &uniform_buffer : info["UniformBuffers"])
    {
        auto &descriptorInfo = mShaderReflectInfo[uniform_buffer["set"].get<uint32_t>()];
        auto &descriptor = descriptorInfo.descriptors.emplace_back();
        descriptor.offset = 0;
        descriptor.buffer = nullptr;
        descriptor.descType = DescriptorType::UniformBuffer;
        descriptor.name = uniform_buffer["name"].get<std::string>();
        descriptor.binding = uniform_buffer["binding"].get<uint32_t>();
        descriptor.shaderType = static_cast<ShaderType>(uniform_buffer["shaderType"].get<uint8_t>());
        for (auto &json_member : uniform_buffer["members"])
        {
            auto &member = descriptor.mMembers.emplace_back();
            member.size = json_member["size"].get<uint32_t>();
            member.offset = json_member["offset"].get<uint32_t>();
            member.type = static_cast<ShaderDataType>(json_member["type"].get<uint8_t>());
            member.fullName = json_member["fullname"].get<std::string>();
            member.name = json_member["name"].get<std::string>();
        }
    }

    for (auto &pushConstant : info["PushConstants"])
    {
        // Todo
    }

    int imageCount[16] = {0};
    int bufferCount[16] = {0};

    for (int i = 0; i < mShaderReflectInfo.size(); i++)
    {
        auto &descriptorInfo = mShaderReflectInfo[i];
        for (auto &descriptor : descriptorInfo.descriptors)
        {
            if (descriptor.descType == DescriptorType::ImageSampler)
            {
                imageCount[i]++;

                if (i > 0)
                    descriptor.binding = descriptor.binding + imageCount[i - 1];
            }
            else if (descriptor.descType == DescriptorType::UniformBuffer)
            {
                bufferCount[i]++;

                if (i > 0)
                    descriptor.binding = descriptor.binding + bufferCount[i - 1];
            }
        }
    }
}

void GLShader::SetUniform(const std::string &name, bool value)
{
    GlCall(glUniform1i(glGetUniformLocation(mHandle, name.c_str()), static_cast<int>(value)));
}

void GLShader::SetUniform(const std::string &name, int value)
{
    GlCall(glUniform1i(glGetUniformLocation(mHandle, name.c_str()), value));
}

void GLShader::SetUniform(const std::string &name, unsigned int value)
{
    GlCall(glUniform1ui(glGetUniformLocation(mHandle, name.c_str()), value));
}

void GLShader::SetUniform(const std::string &name, float value)
{
    GlCall(glUniform1f(glGetUniformLocation(mHandle, name.c_str()), value));
}

void GLShader::SetUniform(const std::string &name, glm::vec2 vec2)
{
    GlCall(glUniform2fv(glGetUniformLocation(mHandle, name.c_str()), 1, glm::value_ptr(vec2)));
}

void GLShader::SetUniform(const std::string &name, glm::vec3 vec3)
{
    GlCall(glUniform3fv(glGetUniformLocation(mHandle, name.c_str()), 1, glm::value_ptr(vec3)));
}

void GLShader::SetUniform(const std::string &name, glm::vec4 vec4)
{
    GlCall(glUniform4fv(glGetUniformLocation(mHandle, name.c_str()), 1, glm::value_ptr(vec4)));
}

void GLShader::SetUniform(const std::string &name, glm::mat3 mat3)
{
    GlCall(glUniformMatrix3fv(glGetUniformLocation(mHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat3)));
}

void GLShader::SetUniform(const std::string &name, glm::mat4 mat4)
{
    GlCall(glUniformMatrix4fv(glGetUniformLocation(mHandle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4)));
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
        GLint length;
        GlCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> error(length);
        GlCall(glGetShaderInfoLog(shader, length, &length, error.data()));
        std::string errorMessage(error.data(), length);
        int32_t lineNumber;
        sscanf(error.data(), "%*s %*d:%d", &lineNumber);
        auto id_type = static_cast<uint32_t>(type);
        info.message[id_type] = "Shader compilation failed: ";
        info.message[id_type] += std::string("Failed to compile ") + Utilities::GetShaderTypeString(type) + " shader!\n";

        info.line[id_type] = lineNumber;
        info.message[id_type] += errorMessage;
        GlCall(glDeleteShader(shader));

        pluto::log<pluto::Error>("OpenGL Shader [%s]: Compilation failed \n %s", source.first.c_str(), info.message[id_type].c_str());
        return 0;
    }
    return shader;
}

uint32_t CompileAll(std::map<ShaderType, std::pair<std::string, std::string>> &sources)
{
    OpenGL::ShaderErrorInfo error;
    uint32_t program = glCreateProgram();
    std::vector<GLuint> shaders;
    std::string glVersion;

    for (auto &source : sources)
    {
        shaders.push_back(CompileShader(source.first, source.second, program, error));
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
