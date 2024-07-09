#pragma once
#include "Graphics/RHI/Shader.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <map>

namespace spirv_cross
{
    class CompilerGLSL;
}

namespace pluto
{
    namespace Graphics
    {
        namespace OpenGL
        {
            struct ShaderErrorInfo
            {
                ShaderErrorInfo() = default;
                std::string shader;
                std::string message[6];
                uint32_t line[6];
            };
        }

        class GLShader : public Shader
        {
            friend class GLContext;

        public:
            GLShader(GLShader::Properties *&&pProperties);
            ~GLShader();

        public:
            bool IsCompiled();

        protected:
            void SetUniform(const std::string &name, bool value);
            void SetUniform(const std::string &name, int value);
            void SetUniform(const std::string &name, unsigned int value);
            void SetUniform(const std::string &name, float value);
            void SetUniform(const std::string &name, glm::vec2 vec2);
            void SetUniform(const std::string &name, glm::vec3 vec3);
            void SetUniform(const std::string &name, glm::vec4 vec4);
            void SetUniform(const std::string &name, glm::mat3 mat3);
            void SetUniform(const std::string &name, glm::mat4 mat4);

        private:
            bool mCompiled;
            uint32_t mHandle;
            std::vector<ShaderType> mShaderTypes;
            std::vector<spirv_cross::CompilerGLSL *> mShaderCompilers;
            // Graphics::VertexInputDescription m_Layout;
        };
    }
}
