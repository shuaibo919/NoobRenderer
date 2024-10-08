/* OpenGL Shader */
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/RHI/Shader.h"
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
    : Shader(ctx, std::move(pProperties)), mCompiled(false), mHandle(0)
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
            this->ReadReflectInfo(shader["reflection"], type);
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

void GLShader::Bind() const
{
    if (this->sCurrently != this)
    {
        GlCall(glUseProgram(mHandle));
        this->sCurrently = const_cast<GLShader *>(this);
    }
}

void GLShader::Unbind() const
{
    if (this->sCurrently == this)
    {
        GlCall(glUseProgram(0));
        this->sCurrently = nullptr;
    }
}

bool GLShader::IsCompiled()
{
    return mCompiled;
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

        // fix
#ifdef _MSC_VER
        sscanf_s(error.data(), "%*s %*d:%d", &lineNumber);
#else
        sscanf(error.data(), "%*s %*d:%d", &lineNumber);
#endif
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
