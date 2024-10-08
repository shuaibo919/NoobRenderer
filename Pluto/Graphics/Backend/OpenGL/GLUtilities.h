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
                case ElementType::Byte:
                case ElementType::Byte2:
                case ElementType::Byte3:
                case ElementType::Byte4:
                    return GL_BYTE;
                case ElementType::Ubyte:
                case ElementType::Ubyte2:
                case ElementType::Ubyte3:
                case ElementType::Ubyte4:
                    return GL_UNSIGNED_BYTE;
                case ElementType::Short:
                case ElementType::Short2:
                case ElementType::Short3:
                case ElementType::Short4:
                    return GL_SHORT;
                case ElementType::Ushort:
                case ElementType::Ushort2:
                case ElementType::Ushort3:
                case ElementType::Ushort4:
                    return GL_UNSIGNED_SHORT;
                case ElementType::Int:
                    return GL_INT;
                case ElementType::Uint:
                    return GL_UNSIGNED_INT;
                case ElementType::Float:
                case ElementType::Float2:
                case ElementType::Float3:
                case ElementType::Float4:
                    return GL_FLOAT;
                case ElementType::Half:
                case ElementType::Half2:
                case ElementType::Half3:
                case ElementType::Half4:
                    return GL_HALF_FLOAT;
                default:
                    break;
                }
                return GL_FLOAT;
            }

            inline constexpr GLenum GetDataType(DataType dataType)
            {
                switch (dataType)
                {
                case DataType::Float:
                    return GL_FLOAT;
                case DataType::UnsignedInt:
                    return GL_UNSIGNED_INT;
                case DataType::UnsignedByte:
                    return GL_UNSIGNED_BYTE;
                default:
                    break;
                }
                return 0;
            }

            inline constexpr GLenum GetDataType(RHIFormat format)
            {
                switch (format)
                {
                case RHIFormat::R8Unorm:
                case RHIFormat::R8G8Unorm:
                case RHIFormat::R8G8B8Unorm:
                case RHIFormat::R8G8B8A8Unorm:
                case RHIFormat::R10G10B10A2Unorm:
                    return GL_UNSIGNED_BYTE;
                case RHIFormat::R16Float:
                case RHIFormat::R16G16Float:
                case RHIFormat::R11G11B10Float:
                case RHIFormat::R16G16B16Float:
                case RHIFormat::R16G16B16A16Float:
                case RHIFormat::R32G32B32Float:
                case RHIFormat::R32G32B32A32Float:
                    return GL_FLOAT;
                default:
                    return GL_UNSIGNED_BYTE;
                }
            }

            inline constexpr GLint GetElementCount(ElementType type)
            {
                return static_cast<GLint>(Utilities::GetElementCount(type));
            }
            inline constexpr GLenum GetBufferUsage(BufferUsage usage)
            {
                switch (usage)
                {
                case BufferUsage::Static:
                    return GL_STATIC_DRAW;
                case BufferUsage::Dynamic:
                    return GL_DYNAMIC_DRAW;
                case BufferUsage::Stream:
                    return GL_STREAM_DRAW;
                }
                return 0;
            }
            inline constexpr GLint GetTextureWrap(const TextureWrap wrap)
            {
                switch (wrap)
                {
                case TextureWrap::Clmap:
                    return GL_CLAMP;
                case TextureWrap::ClampToBorder:
                    return GL_CLAMP_TO_BORDER;
                case TextureWrap::ClampToedge:
                    return GL_CLAMP_TO_EDGE;
                case TextureWrap::Repeat:
                    return GL_REPEAT;
                case TextureWrap::MirroredRepeat:
                    return GL_MIRRORED_REPEAT;
                default:
                    return 0;
                }
            }
            inline constexpr GLint GetTextureFormatSpecific(const RHIFormat format, bool srgb)
            {
                switch (format)
                {
                case RHIFormat::R8Unorm:
                    return GL_R8;
                case RHIFormat::R8G8Unorm:
                    return GL_RG8;
                case RHIFormat::R8G8B8Unorm:
                    return srgb ? GL_SRGB8 : GL_RGB8;
                case RHIFormat::R8G8B8A8Unorm:
                    return srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                case RHIFormat::R11G11B10Float:
                    return GL_R11F_G11F_B10F;
                case RHIFormat::R10G10B10A2Unorm:
                    return GL_RGB10_A2;
                case RHIFormat::R16G16Float:
                    return GL_RG16F;
                case RHIFormat::R16G16B16Float:
                    return GL_RGB16F;
                case RHIFormat::R16G16B16A16Float:
                    return GL_RGBA16F;
                case RHIFormat::R32G32B32Float:
                    return GL_RGB32F;
                case RHIFormat::R32G32B32A32Float:
                    return GL_RGBA32F;
                case RHIFormat::Depth24UnormStencil8UInt:
                    return GL_DEPTH24_STENCIL8;
                case RHIFormat::Depth32Float:
                    return GL_DEPTH_COMPONENT32F;
                case RHIFormat::Depth32FloatStencil8UInt:
                    return GL_DEPTH32F_STENCIL8;
                default:
                    return 0;
                }
            }
            inline constexpr GLenum GetTextureFormat(const GLint format)
            {
                switch (format)
                {
                case GL_SRGB8:
                    return GL_RGB;
                case GL_SRGB8_ALPHA8:
                    return GL_RGBA;
                case GL_RGBA:
                    return GL_RGBA;
                case GL_RGB:
                    return GL_RGB;
                case GL_R8:
                    return GL_RED;
                case GL_RG8:
                    return GL_RG;
                case GL_RGB8:
                    return GL_RGB;
                case GL_RGBA8:
                    return GL_RGBA;
                case GL_RGB16:
                    return GL_RGB;
                case GL_RG16F:
                    return GL_RG;
                case GL_RGBA16:
                    return GL_RGBA;
                case GL_RGBA16F:
                    return GL_RGBA;
                case GL_RGB32F:
                    return GL_RGB;
                case GL_RGBA32F:
                    return GL_RGBA;
                case GL_SRGB:
                    return GL_RGB;
                case GL_SRGB_ALPHA:
                    return GL_RGBA;
                case GL_R11F_G11F_B10F:
                    return GL_RGB;
                case GL_RGB10_A2:
                    return GL_RGBA;
                case GL_LUMINANCE:
                    return GL_LUMINANCE;
                case GL_LUMINANCE_ALPHA:
                    return GL_LUMINANCE_ALPHA;

                default:
                    return 0;
                }
            }
            inline static GLuint PtrToGLuint(void *ptr)
            {
                void *clang[1];
                clang[0] = ptr;
                return *(GLuint *)clang;
            }

            inline constexpr GLenum GetShaderType(ShaderType type)
            {
                switch (type)
                {
                case ShaderType::Vertex:
                    return GL_VERTEX_SHADER;
                case ShaderType::Fragment:
                    return GL_FRAGMENT_SHADER;
                case ShaderType::Geometry:
                    return GL_GEOMETRY_SHADER;
                case ShaderType::Compute:
                    return GL_COMPUTE_SHADER;
                default:
                    break;
                }
                return 0;
            }

            inline constexpr uint32_t GetRendererBuffer(uint32_t buffer)
            {
                uint32_t result = 0;
                if (buffer & RendererBufferType::RenderBufferColor)
                    result |= GL_COLOR_BUFFER_BIT;
                if (buffer & RendererBufferType::RenderBufferDepth)
                    result |= GL_DEPTH_BUFFER_BIT;
                if (buffer & RendererBufferType::RenderBufferStencil)
                    result |= GL_STENCIL_BUFFER_BIT;
                return result;
            }

            inline constexpr GLenum GetDrawType(DrawType drawType)
            {
                switch (drawType)
                {
                case DrawType::Point:
                    return GL_POINTS;
                case DrawType::Lines:
                    return GL_LINES;
                case DrawType::Triangle:
                    return GL_TRIANGLES;
                default:

                    break;
                }
                return 0;
            }
        }
    }
}
