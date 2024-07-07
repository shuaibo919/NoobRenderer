#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/RHI/Utilities.h"
#include "Graphics/Backend/OpenGL/GL.h"
namespace pluto
{
    namespace Graphics
    {
        namespace GLUtilities
        {
            inline constexpr GLenum GetDataType(ElementType type)
            {
                switch (type)
                {
                case ElementType::BYTE:
                case ElementType::BYTE2:
                case ElementType::BYTE3:
                case ElementType::BYTE4:
                    return GL_BYTE;
                case ElementType::UBYTE:
                case ElementType::UBYTE2:
                case ElementType::UBYTE3:
                case ElementType::UBYTE4:
                    return GL_UNSIGNED_BYTE;
                case ElementType::SHORT:
                case ElementType::SHORT2:
                case ElementType::SHORT3:
                case ElementType::SHORT4:
                    return GL_SHORT;
                case ElementType::USHORT:
                case ElementType::USHORT2:
                case ElementType::USHORT3:
                case ElementType::USHORT4:
                    return GL_UNSIGNED_SHORT;
                case ElementType::INT:
                    return GL_INT;
                case ElementType::UINT:
                    return GL_UNSIGNED_INT;
                case ElementType::FLOAT:
                case ElementType::FLOAT2:
                case ElementType::FLOAT3:
                case ElementType::FLOAT4:
                    return GL_FLOAT;
                case ElementType::HALF:
                case ElementType::HALF2:
                case ElementType::HALF3:
                case ElementType::HALF4:
                    return GL_HALF_FLOAT;
                default:
                    break;
                }
                return GL_FLOAT;
            }
            inline constexpr GLint GetElementCount(ElementType type)
            {
                return static_cast<GLint>(Utilities::GetElementCount(type));
            }
            inline constexpr GLenum GetBufferUsage(BufferUsage usage)
            {
                switch (usage)
                {
                case BufferUsage::STATIC:
                    return GL_STATIC_DRAW;
                case BufferUsage::DYNAMIC:
                    return GL_DYNAMIC_DRAW;
                case BufferUsage::STREAM:
                    return GL_STREAM_DRAW;
                }
                return 0;
            }

            // uint32_t FormatToGL(RHIFormat format, bool srgb = true);
            // uint32_t TextureWrapToGL(TextureWrap wrap);
            // uint32_t FormatToInternalFormat(uint32_t format);
            // uint32_t StencilTypeToGL(const StencilType type);
            // uint32_t GetGLTypefromFormat(RHIFormat format);

            // uint32_t RendererBufferToGL(uint32_t buffer);
            // uint32_t RendererBlendFunctionToGL(RendererBlendFunction function);
            // uint32_t DataTypeToGL(DataType dataType);
            // uint32_t DrawTypeToGL(DrawType drawType);
        }
    }
}
