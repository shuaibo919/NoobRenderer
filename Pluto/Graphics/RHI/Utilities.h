#pragma once
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        namespace Utilities
        {
            constexpr inline uint16_t GetElementCount(ElementType type) noexcept
            {
                switch (type)
                {
                case ElementType::BYTE:
                case ElementType::UBYTE:
                case ElementType::SHORT:
                case ElementType::USHORT:
                case ElementType::INT:
                case ElementType::UINT:
                case ElementType::FLOAT:
                case ElementType::HALF:
                    return 1;
                case ElementType::FLOAT2:
                case ElementType::HALF2:
                case ElementType::BYTE2:
                case ElementType::UBYTE2:
                case ElementType::SHORT2:
                case ElementType::USHORT2:
                    return 2;
                case ElementType::FLOAT3:
                case ElementType::HALF3:
                case ElementType::BYTE3:
                case ElementType::UBYTE3:
                case ElementType::SHORT3:
                case ElementType::USHORT3:
                    return 3;
                case ElementType::FLOAT4:
                case ElementType::HALF4:
                case ElementType::BYTE4:
                case ElementType::UBYTE4:
                case ElementType::SHORT4:
                case ElementType::USHORT4:
                    return 4;
                default:
                    break;
                }
                return 0;
            }

            constexpr inline uint32_t GetBitsFromFormat(const RHIFormat format)
            {
                switch (format)
                {
                case RHIFormat::R8_Unorm:
                    return 8;
                case RHIFormat::D16_Unorm:
                    return 16;
                case RHIFormat::R8G8_Unorm:
                    return 16;
                case RHIFormat::R8G8B8_Unorm:
                    return 24;
                case RHIFormat::R16G16B16_Float:
                    return 48;
                case RHIFormat::R32G32B32_Float:
                    return 96;
                case RHIFormat::R8G8B8A8_Unorm:
                    return 32;
                case RHIFormat::R16G16B16A16_Float:
                    return 64;
                case RHIFormat::R32G32B32A32_Float:
                    return 128;
                default:
                    return 32;
                }
            }

            constexpr inline RHIFormat BitsToFormat(uint32_t bits)
            {
                switch (bits)
                {
                case 8:
                    return RHIFormat::R8_Unorm;
                case 16:
                    return RHIFormat::R8G8_Unorm;
                case 24:
                    return RHIFormat::R8G8B8_Unorm;
                case 32:
                    return RHIFormat::R8G8B8A8_Unorm;
                case 48:
                    return RHIFormat::R16G16B16_Float;
                case 64:
                    return RHIFormat::R16G16B16A16_Float;
                case 96:
                    return RHIFormat::R32G32B32_Float;
                case 128:
                    return RHIFormat::R32G32B32A32_Float;
                default:

                    return RHIFormat::R8G8B8A8_Unorm;
                }
            }

            constexpr inline uint32_t BitsToChannelCount(uint32_t bits)
            {
                switch (bits)
                {
                case 8:
                    return 1;
                case 16:
                    return 2;
                case 24:
                    return 3;
                case 32:
                    return 4;
                case 48:
                    return 3;
                case 64:
                    return 4;
                case 96:
                    return 3;
                case 128:
                    return 4;
                default:
                    return 4;
                }
            }

            constexpr inline uint32_t CalculateMipMapCount(uint32_t width, uint32_t height)
            {
                uint32_t levels = 1;
                while ((width | height) >> levels)
                    levels++;

                return levels;
            }

            constexpr inline bool IsDepthStencilFormat(RHIFormat format)
            {
                return format == RHIFormat::D24_Unorm_S8_UInt || format == RHIFormat::D16_Unorm_S8_UInt || format == RHIFormat::D32_Float_S8_UInt;
            }

            constexpr inline bool IsDepthFormat(RHIFormat format)
            {
                return format == RHIFormat::D16_Unorm || format == RHIFormat::D32_Float || format == RHIFormat::D24_Unorm_S8_UInt || format == RHIFormat::D16_Unorm_S8_UInt || format == RHIFormat::D32_Float_S8_UInt;
            }

            constexpr inline bool IsStencilFormat(RHIFormat format)
            {
                return format == RHIFormat::D24_Unorm_S8_UInt || format == RHIFormat::D16_Unorm_S8_UInt || format == RHIFormat::D32_Float_S8_UInt;
            }
            constexpr inline uint8_t GetStrideFromFormat(const RHIFormat format)
            {
                switch (format)
                {
                case RHIFormat::R8_Unorm:
                case RHIFormat::D16_Unorm:
                    return 1;
                case RHIFormat::R8G8_Unorm:
                    return 2;
                case RHIFormat::R8G8B8_Unorm:
                case RHIFormat::R16G16B16_Float:
                case RHIFormat::R32G32B32_Float:
                    return 3;
                case RHIFormat::R8G8B8A8_Unorm:
                case RHIFormat::R16G16B16A16_Float:
                case RHIFormat::R32G32B32A32_Float:
                    return 4;
                default:
                    return 0;
                }
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
