#pragma once
#include "Graphics/RHI/Shader.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <map>

namespace pluto
{
    namespace Graphics
    {
        class VKShader : public Shader
        {
            friend class VKContext;

        public:
            VKShader(RenderContext *ctx, VKShader::Properties *&&pProperties);
            ~VKShader();

        public:
            void Bind() const override;
            void Unbind() const override;
            bool IsCompiled() override;
        };
    }
}
