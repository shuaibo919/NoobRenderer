#pragma once
#include "Graphics/RHI/Shader.h"
// #include "Graphics/RHI/BufferLayout.h"
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
        struct GLShaderErrorInfo
        {
            GLShaderErrorInfo()
                : shader(0){};
            uint32_t shader;
            std::string message[6];
            uint32_t line[6];
        };

        class GLShader : public Shader
        {

            // Graphics::BufferLayout mLayout;
        };
    }
}
