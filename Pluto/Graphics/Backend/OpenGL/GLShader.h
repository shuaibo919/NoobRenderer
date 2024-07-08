#pragma once
#include "Graphics/RHI/Shader.h"
#include "Graphics/RHI/BufferLayout.hpp"
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
            void BindPushConstants(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Pipeline> pipeline);

        protected:
            void ReflectFromShaderData(const uint32_t *data, uint32_t size, ShaderType type, std::map<ShaderType, std::string> &sources);
            static uint32_t CompileAll(std::map<ShaderType, std::string> *sources, OpenGL::ShaderErrorInfo &info);
            static uint32_t CompileShader(ShaderType type, std::string source, uint32_t program, OpenGL::ShaderErrorInfo &info);

        private:
            uint32_t mHandle;
            std::string mSource;

            std::vector<ShaderType> mShaderTypes;
            std::map<uint32_t, DescriptorSetInfo> mDescriptorInfos;

            bool CreateLocations();
            bool SetUniformLocation(const std::string &szName, bool pc = false);

            std::map<std::string, uint32_t> msUniformBlockLocations;
            std::map<uint32_t, uint32_t> mSampledImageLocations;
            std::map<uint32_t, uint32_t> mUniformLocations;

            std::vector<spirv_cross::CompilerGLSL *> mShaderCompilers;
            std::vector<PushConstant> mPushConstants;
            std::vector<std::pair<GLUniformBuffer *, uint32_t>> mPushConstantsBuffers;
            BufferLayout *mLayout;
            // Graphics::VertexInputDescription m_Layout;
        };
    }
}
