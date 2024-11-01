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
        class VertexBuffer;
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
            Front = 0,
            Back,
            FrontAndBack,
            None
        };

        enum class PolygonMode : uint8_t
        {
            Fill = 0,
            Line,
            Point
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
            None = 0,
            Repeat,
            Clmap,
            MirroredRepeat,
            ClampToedge,
            ClampToBorder
        };

        enum class TextureFilter : uint8_t
        {
            None = 0,
            Linear,
            Nearest
        };

        enum class TextureUsage
        {
            None,
        };

        enum class RHIFormat : uint8_t
        {
            None = 0,
            R8Unorm,
            R8G8Unorm,
            R8G8B8Unorm,
            R8G8B8A8Unorm,

            R8UInt,

            R11G11B10Float,
            R10G10B10A2Unorm,

            R32Int,
            R32G32Int,
            R32G32B32Int,
            R32G32B32A32Int,

            R32UInt,
            R32G32UInt,
            R32G32B32UInt,
            R32G32B32A32UInt,

            R16Float,
            R16G16Float,
            R16G16B16Float,
            R16G16B16A16Float,

            R32Float,
            R32G32Float,
            R32G32B32Float,
            R32G32B32A32Float,

            Depth16Unorm,
            Depth32Float,
            Depth16UnormStencil8UInt,
            Depth24UnormStencil8UInt,
            Depth32FloatStencil8UInt
        };

        enum class BufferUsage : uint8_t
        {
            Static,
            Dynamic,
            Stream
        };

        enum class DescriptorType : uint8_t
        {
            UniformBuffer,
            UniformBufferDynamic,
            ImageSampler,
            ImageStorage
        };

        enum class ShaderDataType : uint8_t
        {
            None = 0,
            Float32,
            Vec2,
            Vec3,
            Vec4,
            Ivec2,
            Ivec3,
            Ivec4,
            Mat3,
            Mat4,
            Int32,
            Int,
            Uint,
            Bool,
            Struct
        };

        enum class ReflectDataType : uint8_t
        {
            Unkown,
            Bit8Int,
            Bit8Uint,
            Bit16SInt,
            Bit16Uint,
            Bit16SFloat,
            Bit32Int,
            Bit32Uint,
            Bit32SFloat,
            Bit64SInt,
            Bit64Uint,
            Bit64SFloat,
            Vec2Bit8Int,
            Vec2Bit8Uint,
            Vec2Bit16SInt,
            Vec2Bit16Uint,
            Vec2Bit16SFloat,
            Vec2Bit32Int,
            Vec2Bit32Uint,
            Vec2Bit32SFloat,
            Vec2Bit64SInt,
            Vec2Bit64Uint,
            Vec2Bit64SFloat,
            Vec3Bit8Int,
            Vec3Bit8Uint,
            Vec3Bit16SInt,
            Vec3Bit16Uint,
            Vec3Bit16SFloat,
            Vec3Bit32Int,
            Vec3Bit32Uint,
            Vec3Bit32SFloat,
            Vec3Bit64SInt,
            Vec3Bit64Uint,
            Vec3Bit64SFloat,
            Vec4Bit8Int,
            Vec4Bit8Uint,
            Vec4Bit16SInt,
            Vec4Bit16Uint,
            Vec4Bit16SFloat,
            Vec4Bit32Int,
            Vec4Bit32Uint,
            Vec4Bit32SFloat,
            Vec4Bit64SInt,
            Vec4Bit64Uint,
            Vec4Bit64SFloat
        };

        enum class ShaderType : uint8_t
        {
            Vertex = 0,
            Fragment,
            Geometry,
            Compute,
            Unkown
        };

        enum class AttachmentType : uint8_t
        {
            Color = 0,
            Depth,
            DepthArray,
            Cube,
            Other
        };

        enum SubPassContents : uint8_t
        {
            Inline = 0,
            Secondary
        };

        enum TextureFlags : uint8_t
        {
            TextureSampled,
            TextureStorage,
            TextureRenderTarget,
            TextureDepthStencil,
            TextureDepthStencilReadOnly,
            TextureCreateMips,
            TextureMipViews
        };

        enum RendererBufferType : uint8_t
        {
            RenderBufferColor = 1 << 0,
            RenderBufferDepth = 1 << 1,
            RenderBufferStencil = 1 << 2
        };

        enum class DrawType : uint8_t
        {
            Point = 0,
            Triangle,
            TriangleStrip,
            Lines
        };

        enum class StencilType : uint8_t
        {
            Equal = 0,
            NotEqual,
            Keep,
            Replace,
            Zero,
            Always,
            None
        };

        enum class RenderMode : uint8_t
        {
            Fill = 0,
            WireFrame
        };

        enum class DataType : uint8_t
        {
            Float = 0,
            UnsignedInt,
            UnsignedByte
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
            Position = 0,    //!< XYZ position (float3)
            Tangents = 1,    //!< tangent, bitangent and normal, encoded as a quaternion (float4)
            Color = 2,       //!< vertex color (float4)
            Uv0 = 3,         //!< texture coordinates (float2)
            Uv1 = 4,         //!< texture coordinates (float2)
            BoneIndices = 5, //!< indices of 4 bones, as unsigned integers (uvec4)
            BoneWeights = 6, //!< weights of the 4 bones (normalized float4)
            Custom0 = 8,
            Custom1 = 9,
            Custom2 = 10,
            Custom3 = 11,
            Custom4 = 12,
            Custom5 = 13,
            Custom6 = 14,
            Custom7 = 15,
        };

        enum class ElementType : uint8_t
        {
            Byte,
            Byte2,
            Byte3,
            Byte4,
            Ubyte,
            Ubyte2,
            Ubyte3,
            Ubyte4,
            Short,
            Short2,
            Short3,
            Short4,
            Ushort,
            Ushort2,
            Ushort3,
            Ushort4,
            Int,
            Uint,
            Float,
            Float2,
            Float3,
            Float4,
            Half,
            Half2,
            Half3,
            Half4,
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
            std::vector<uint32_t> counts;
            DescriptorLayoutInfo(DescriptorType _type, ShaderType _stage, uint32_t _binding, 
                            uint32_t _setID, std::vector<uint32_t> _counts)
                            : type(_type), stage(_stage), binding(_binding), setID(_setID), counts(_counts) {}
        };

        struct DescriptorLayout
        {
            uint32_t count;
            DescriptorLayoutInfo *layoutInfo;
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

        struct Descriptor
        {
            SharedPtr<Texture> texture;
            SharedPtr<UniformBuffer> ubo;

            uint32_t offset;
            uint32_t size;
            uint32_t binding;
            uint32_t mipLevel = 0;
            std::string name;

            AttachmentType attachmentType;
            DescriptorType descType = DescriptorType::ImageSampler;
            ShaderType shaderType;

            std::vector<BufferMemberInfo> mMembers;
        };

        struct DescriptorSetInfo
        {
            std::vector<Descriptor> descriptors;
        };

        enum class PhysicalDeviceType
        {
            Discrete,
            Integrated,
            Virtual,
            Cpu,
            Unknown
        };

        enum class CommandBufferState : uint8_t
        {
            Idle,
            Recording,
            Ended,
            Submitted
        };

        enum class CommandBufferUsageType : uint8_t
        {
            OneTimeSubmit,
            RecycleSubmit,
        };
    }
}
