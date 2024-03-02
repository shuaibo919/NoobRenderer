#pragma once

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"

namespace NoobRenderer
{
    /// @brief shader
    using ShaderIndex = unsigned int;
    class ShaderProgram
    {
        friend class ShaderManager;

    public:
        using Ptr = std::shared_ptr<ShaderProgram>;
        inline void Use() { glUseProgram(m_programID); }

    private:
        GLuint m_programID;
        std::string vShaderName;
        GLuint vShaderID;
        std::string fShaderName;
        GLuint fShaderID;
        std::string gShaderName;
        GLuint gShaderID;
        bool m_use_geometry_shader;
        bool CompileShader(GLuint &programID, GLuint &shaderID, const std::string &shaderSrc, GLenum type);

    public:
        ShaderProgram();
        ~ShaderProgram();
        void SetVertexShader(const std::string &name);
        std::string GetVertexShaderName();

        void SetFragmentShader(const std::string &name);
        std::string GetFragmentShaderName();

        void SetGeometryShader(const std::string &name);
        std::string GetGeometryShaderName();

        void BuildShaderProgram(bool use_geometry_shader = false);
        GLuint GetShaderProgram();

        bool GetGeometryEnabled() { return m_use_geometry_shader; }

        void SetUniform(const GLchar *name, bool value);
        void SetUniform(const GLchar *name, int value);
        void SetUniform(const GLchar *name, unsigned int value);
        void SetUniform(const GLchar *name, float value);
        void SetUniform(const GLchar *name, glm::vec2 vec2);
        void SetUniform(const GLchar *name, glm::vec3 vec3);
        void SetUniform(const GLchar *name, glm::vec4 vec4);
        void SetUniform(const GLchar *name, glm::mat3 mat3);
        void SetUniform(const GLchar *name, glm::mat4 mat4);

    public:
        inline void SetUniform(const std::string &name, bool value) { SetUniform(name.c_str(), value); }
        void SetUniform(const std::string &name, int value) { SetUniform(name.c_str(), value); }
        void SetUniform(const std::string &name, unsigned int value) { SetUniform(name.c_str(), value); }
        void SetUniform(const std::string &name, float value) { SetUniform(name.c_str(), value); }
        void SetUniform(const std::string &name, glm::vec2 vec2) { SetUniform(name.c_str(), vec2); }
        void SetUniform(const std::string &name, glm::vec3 vec3) { SetUniform(name.c_str(), vec3); }
        void SetUniform(const std::string &name, glm::vec4 vec4) { SetUniform(name.c_str(), vec4); }
        void SetUniform(const std::string &name, glm::mat3 mat3) { SetUniform(name.c_str(), mat3); }
        void SetUniform(const std::string &name, glm::mat4 mat4) { SetUniform(name.c_str(), mat4); }
    };
}