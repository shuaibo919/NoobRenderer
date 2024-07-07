#pragma once
#include "Core/Log.hpp"
#include "Core/Asset.h"
#include "Core/Base.hpp"

namespace pluto
{
    namespace Graphics
    {
        class Shader;
        class RenderPass;
        class CommandBuffer;
        class DescriptorSet;
        class Pipeline;
        class Shader;
        class UniformBuffer;
        class Framebuffer;
        class RenderPass;
        class GraphicsContext;
        class Texture;
        class Texture2D;
        class TextureCube;
        class Texture2DArray;
        class Texture3D;

        enum class CullMode : uint8_t
        {
            FRONT = 0,
            BACK,
            FRONTANDBACK,
            NONE
        };

        enum class PolygonMode : uint8_t
        {
            FILL = 0,
            LINE,
            POINT
        };

        enum class BlendMode : uint8_t
        {
            None = 0,
            OneZero,
            ZeroSrcColor,
            SrcAlphaOneMinusSrcAlpha,
            SrcAlphaOne
        };

        enum class TextureWrap : uint8_t
        {
            NONE = 0,
            REPEAT,
            CLAMP,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER
        };

        enum class TextureFilter : uint8_t
        {
            NONE = 0,
            LINEAR,
            NEAREST
        };

        enum class TextureType
        {
            None,
            XX
        };

        enum class RHIFormat : uint8_t
        {
            NONE = 0,
            R8_Unorm,
            R8G8_Unorm,
            R8G8B8_Unorm,
            R8G8B8A8_Unorm,

            R8_UInt,

            R11G11B10_Float,
            R10G10B10A2_Unorm,

            R32_Int,
            R32G32_Int,
            R32G32B32_Int,
            R32G32B32A32_Int,

            R32_UInt,
            R32G32_UInt,
            R32G32B32_UInt,
            R32G32B32A32_UInt,

            R16_Float,
            R16G16_Float,
            R16G16B16_Float,
            R16G16B16A16_Float,

            R32_Float,
            R32G32_Float,
            R32G32B32_Float,
            R32G32B32A32_Float,

            D16_Unorm,
            D32_Float,
            D16_Unorm_S8_UInt,
            D24_Unorm_S8_UInt,
            D32_Float_S8_UInt,
            SCREEN
        };

        enum class BufferUsage : uint8_t
        {
            STATIC,
            DYNAMIC,
            STREAM
        };

        enum class DescriptorType : uint8_t
        {
            UNIFORM_BUFFER,
            UNIFORM_BUFFER_DYNAMIC,
            IMAGE_SAMPLER,
            IMAGE_STORAGE
        };

        enum class ShaderDataType : uint8_t
        {
            NONE = 0,
            FLOAT32,
            VEC2,
            VEC3,
            VEC4,
            IVEC2,
            IVEC3,
            IVEC4,
            MAT3,
            MAT4,
            INT32,
            INT,
            UINT,
            BOOL,
            STRUCT,
            MAT4ARRAY
        };

        enum class ShaderType : uint8_t
        {
            VERTEX = 0,
            FRAGMENT,
            GEOMETRY,
            TESSELLATION_CONTROL,
            TESSELLATION_EVALUATION,
            COMPUTE,
            UNKNOWN
        };

        enum class AttachmentType : uint8_t
        {
            COLOUR = 0,
            DEPTH,
            DEPTHARRAY,
            CUBE,
            OTHER
        };

        enum SubPassContents : uint8_t
        {
            INLINE = 0,
            SECONDARY
        };

        enum TextureFlags : uint8_t
        {
            Texture_Sampled,
            Texture_Storage,
            Texture_RenderTarget,
            Texture_DepthStencil,
            Texture_DepthStencilReadOnly,
            Texture_CreateMips,
            Texture_MipViews
        };

        enum RendererBufferType : uint8_t
        {
            RENDERER_BUFFER_COLOUR,
            RENDERER_BUFFER_DEPTH,
            RENDERER_BUFFER_STENCIL,
            RENDERER_BUFFER_NONE

        };

        enum class DrawType : uint8_t
        {
            POINT = 0,
            TRIANGLE,
            LINES
        };

        enum class StencilType : uint8_t
        {
            EQUAL = 0,
            NOTEQUAL,
            KEEP,
            REPLACE,
            ZERO,
            ALWAYS
        };

        enum class PixelPackType : uint8_t
        {
            PACK = 0,
            UNPACK
        };

        enum class RendererBlendFunction : uint8_t
        {
            NONE = 0,
            ZERO,
            ONE,
            SOURCE_ALPHA,
            DESTINATION_ALPHA,
            ONE_MINUS_SOURCE_ALPHA
        };

        enum class RendererBlendEquation : uint8_t
        {
            NONE = 0,
            ADD,
            SUBTRACT
        };

        enum class RenderMode : uint8_t
        {
            FILL = 0,
            WIREFRAME
        };

        enum class DataType : uint8_t
        {
            FLOAT = 0,
            UNSIGNED_INT,
            UNSIGNED_BYTE
        };

        struct BufferMemberInfo
        {
            uint32_t size;
            uint32_t offset;
            ShaderDataType type;
            std::string name;
            std::string fullName;
        };

        enum class VertexAttributeType : uint8_t
        {
            POSITION = 0,     //!< XYZ position (float3)
            TANGENTS = 1,     //!< tangent, bitangent and normal, encoded as a quaternion (float4)
            COLOR = 2,        //!< vertex color (float4)
            UV0 = 3,          //!< texture coordinates (float2)
            UV1 = 4,          //!< texture coordinates (float2)
            BONE_INDICES = 5, //!< indices of 4 bones, as unsigned integers (uvec4)
            BONE_WEIGHTS = 6, //!< weights of the 4 bones (normalized float4)
            CUSTOM0 = 8,
            CUSTOM1 = 9,
            CUSTOM2 = 10,
            CUSTOM3 = 11,
            CUSTOM4 = 12,
            CUSTOM5 = 13,
            CUSTOM6 = 14,
            CUSTOM7 = 15,
        };

        enum class ElementType : uint8_t
        {
            BYTE,
            BYTE2,
            BYTE3,
            BYTE4,
            UBYTE,
            UBYTE2,
            UBYTE3,
            UBYTE4,
            SHORT,
            SHORT2,
            SHORT3,
            SHORT4,
            USHORT,
            USHORT2,
            USHORT3,
            USHORT4,
            INT,
            UINT,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            HALF,
            HALF2,
            HALF3,
            HALF4,
        };

        struct VertexAttribute
        {
            const static uint8_t MAX_VERTEX_COUNT = 12;
            bool normalized;
            uint16_t bufferIndex;
            uint32_t offset;
            uint16_t stride;
            ElementType type;
        };

        struct VertexInputDescription
        {
            uint32_t binding;
            uint32_t location;
            RHIFormat format;
            uint32_t offset;
        };

        struct DescriptorPoolInfo
        {
            DescriptorType type;
            uint32_t size;
        };

        struct DescriptorLayoutInfo
        {
            DescriptorType type;
            ShaderType stage;
            uint32_t binding = 0;
            uint32_t setID = 0;
            uint32_t count = 1;
        };

        struct DescriptorLayout
        {
            uint32_t count;
            DescriptorLayoutInfo *layoutInfo;
        };

        struct Descriptor
        {
            Texture **textures;
            Texture *texture;
            UniformBuffer *buffer;

            uint32_t offset;
            uint32_t size;
            uint32_t binding;
            uint32_t textureCount = 1;
            uint32_t mipLevel = 0;
            std::string name;

            // TextureType textureType;
            DescriptorType type = DescriptorType::IMAGE_SAMPLER;
            ShaderType shaderType;

            std::vector<BufferMemberInfo> mMembers;
        };

        enum class CubeFace : uint8_t
        {
            PositiveX = 0,
            NegativeX,
            PositiveY,
            NegativeY,
            PositiveZ,
            NegativeZ
        };

        struct PushConstant
        {
            uint32_t size;
            ShaderType shaderStage;
            uint8_t *data;
            uint32_t offset = 0;
            std::string name;

            std::vector<BufferMemberInfo> mMembers;

            inline void SetValue(const std::string &name, void *value)
            {
                for (auto &member : mMembers)
                {
                    if (member.name == name)
                    {
                        memcpy(&data[member.offset], value, member.size);
                        return;
                    }
                }
            }

            inline void SetData(void *value)
            {
                memcpy(data, value, size);
            }
        };

        struct DescriptorSetInfo
        {
            std::vector<Descriptor> descriptors;
        };
    }
}
