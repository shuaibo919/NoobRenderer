#ifndef PLUTO_BACKEND_ENUMS_H
#define PLUTO_BACKEND_ENUMS_H

#include <cstdint>
#include <cstddef>

namespace pluto::backend
{
    /// @brief Which graphics api to be used
    enum class Backend : uint8_t
    {
        Default = 0,
        OpenGL = 1,
        Vulkan = 2,
        NotImplemented = 3,
    };

    /// @brief Get the raw string of a backend enum-class
    static constexpr const char *backendToString(Backend backend)
    {
        switch (backend)
        {
        case Backend::NotImplemented:
            return "Not Implemented";
        case Backend::OpenGL:
            return "OpenGL";
        case Backend::Vulkan:
            return "Vulkan";
        default:
            return "Unknown";
        }
    }

    /// @brief Error codes of timer query
    enum class TimerQueryResult : int8_t
    {
        Error = -1,
        NotReady = 0,
        Avaiable = 1,
    };

    /// @brief Bitmask for selecting render buffers
    enum class TargetBufferFlags : uint32_t
    {
        NONE = 0x0u,
        Color0 = 0x00000001u,
        Color1 = 0x00000002u,
        Color2 = 0x00000004u,
        Color3 = 0x00000008u,
        Color4 = 0x00000010u,
        Color5 = 0x00000020u,
        Color6 = 0x00000040u,
        Color7 = 0x00000080u,
        ColorAll = Color0 | Color1 | Color2 | Color3 | Color4 | Color5 | Color6 | Color7,
        Depth = 0x10000000u,
        Stencil = 0x20000000u,
        DepthAndStencil = Depth | Stencil,
        All = ColorAll | Depth | Stencil
    };

    /// @brief Frequency at which a buffer is expected to be modified and used.
    enum class BufferUsage : uint8_t
    {
        Static,
        Dynamic,
    };

    /// @brief Error codes for Fence Class
    enum class FenceStatus : int8_t
    {
        Error = -1,
        Satisfied = 0,
        TimeoutExpired = 1,
    };

    /// @brief Status codes for sync objects
    enum class SyncStatus : int8_t
    {
        Error = -1,
        Signaled = 0,
        NotSignaled = 1,
    };

    /// @brief Primitive types don't change the enums values (made to match OpenGL)
    enum class PrimitiveType : uint8_t
    {
        Points = 0,
        Lines = 1,
        LineStrip = 3,
        Triangles = 4,
        TriangleStrip = 5
    };

    /// @brief Supported uniform types
    enum class UniformType : uint8_t
    {
        Bool,
        Bool2,
        Bool3,
        Bool3,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Uint,
        Uint2,
        Uint3,
        Uint4,
        Mat3,
        Mat4,
        Struct
    };

    /// @brief Supported constant parameter types
    enum class ConstantType : uint8_t
    {
        Int,
        Float,
        Bool
    };

    /// @brief Shader compiler queue type
    enum class CompilerPriorityQueue : uint8_t
    {
        High,
        Low
    };

    /// @brief Texture sampler type
    enum class SamplerType : uint8_t
    {
        Sample2D,
        Sample2DArray,
        SampleCubeMap,
        Sample3D,
    };

    /// @brief Texture sampler format
    enum class SamplerFormat : uint8_t
    {
        Int = 0,
        Uint = 1,
        Float = 2,
    };

    /// @brief Supported element types
    enum class ElementType : uint8_t
    {
        Byte,
        Byte2,
        Byte3,
        Byte4,
        UByte,
        UByte2,
        UByte3,
        UByte4,
        Short,
        Short2,
        Short3,
        Short4,
        UShort,
        UShort2,
        UShort3,
        UShort4,
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

    /// @brief Buffer object binding type
    enum class BufferObjectBinding : uint8_t
    {
        Vertex,
        Uniform
    };

    /// @brief Face culling Mode
    enum class CullingMode : uint8_t
    {
        None,
        Front,
        Back,
        FrontAndBack
    };

    /// @brief Pixel Data Format
    enum class PixelDataFormat : uint8_t
    {
        R,
        RInt,
        RG,
        RGInt,
        RGB,
        RGBInt,
        RGBA,
        RGBAInt,
        Unused,
        DepthComponent,
        DepthStencil,
        Alpha
    };

    /// @brief Pixel Data Type
    enum class PixelDataType : uint8_t
    {
        UByte,
        Byte,
        UShort,
        Short,
        Uint,
        Int,
        Half,
        Float,
    };

    /// @brief Texture Format Type
    enum class TextureFormat : uint16_t
    {
        // 8-bits per element
        R8,
        R8Snorm,
        R8UI,
        R8I,
        Stencil8,

        // 16-bits per element
        R16F,
        R16UI,
        R16I,
        RG8,
        RG8Snorm,
        RG8UI,
        RG8I,
        RGBA4,
        DEPTH16,

        // 24-bits per element
        RGB8,
        SRGB8,
        RGB8Snorm,
        RGB8UI,
        RGB8I,
        Depth24,

        // 32-bits per element
        R32F,
        R32UI,
        R32I,
        RG16F,
        RG16UI,
        RG16I,
        R11F_G11F_B10F,
        RGBA8,
        SRGB8_A8,
        RGBA8Snorm,
        Unused,
        RGB10_A2,
        RGBA8UI,
        RGBA8I,
        Depth32F,
        Depth24Stencil8,
        Depth32FStencil8,

        // 48-bits per element
        RGB16F,
        RGB16UI,
        RGB16I,

        // 64-bits per element
        RG32F,
        RG32UI,
        RG32I,
        RGBA16F,
        RGBA16UI,
        RGBA16I,

        // 96-bits per element
        RGB32F,
        RGB32UI,
        RGB32I,

        // 128-bits per element
        RGBA32F,
        RGBA32UI,
        RGBA32I
    };

    /// @brief  Bitmask describing the intended Texture Usage
    enum class TextureUsage : uint16_t
    {
        None = 0x0000,
        ColorAttachment = 0x0001,        
        DepthAttachment = 0x0002,        
        StencilAttachment = 0x0004,      
        Uploadable = 0x0008,             
        Sampleble = 0x0010,              
        SubpassInput = 0x0020,           
        BlitSrc = 0x0040,                
        BlitDst = 0x0080,                
        Protected = 0x0100,              
        Default = Uploadable | Sampleble 
    };

    /// @brief  Texture Cubemap Face
    enum class TextureCubemapFace : uint8_t
    {
        // don't change the enums values
        PostiveX = 0,  // +x face
        NegativeX = 1, // -x face
        PositiveY = 2, // +y face
        NegativeY = 3, // -y face
        PostiveZ = 4,  // +z face
        NegativeZ = 5, // -z face
    };

    /// @brief  Sampler Wrap mode
    enum class SamplerWrapMode : uint8_t
    {
        ClampToEdge,   
        Repeat,        
        MirroedRepeat, 
    };
    /// @brief Sampler minification filter (don't change the enums values)
    enum class SamplerMinFilter : uint8_t
    {
        Nearest = 0,              
        Linear = 1,               
        NearestMipmapNearest = 2, 
        LinearMipmapNearest = 3,  
        NearestMipmapLinear = 4,  
        LinearMipmapLinear = 5    
    };

    /// @brief Sampler magnification filter (don't change the enums values)
    enum class SamplerMagFilter : uint8_t
    {
        Nearest = 0, 
        Linear = 1,  
    };

    /// @brief Sampler compare mode (don't change the enums values)
    enum class SamplerCompareMode : uint8_t
    {
        None = 0,
        CompareToTexture = 1
    };

    /// @brief comparison function for the depth / stencil sampler (don't change the enums values)
    enum class SamplerCompareFunc : uint8_t
    {
        LessEqual = 0, 
        GreaterEqual,  
        LessThan,      
        GreaterThan,   
        Equal,         
        NotEqual,      
        Always,        
        Never          
    };

    /// @brief  blending equation function
    enum class BlendEquation : uint8_t
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    /// @brief  blending function
    enum class BlendFunction : uint8_t
    {
        Zero,             // func(src,dst) = 0
        One,              // func(src,dst) = 1
        Src,              // func(src,dst) = src
        OneMinusSrc,      // func(src,dst) = 1-src
        Dst,              // func(src,dst) = dst
        OneMinusDst,      // func(src,dst) = 1-dst
        SrcAlpha,         // func(src,dst) = src.a
        OneMinusSrcAlpha, // func(src,dst) = 1-src.a
        DstAlpha,         // func(src,dst) = dst.a
        OneMinusDstAlpha, // func(src,dst) = 1-dst.a
        SrcAlphaSaturate, // func(src,dst) = min(src.a, 1 - dst.a), 1
    };

    /// @brief stencil operation
    enum class StencilOperation : uint8_t
    {
        Keep,
        Zero,
        Invert,
        Replace,
        Increment,
        IncrementWrap,
        Decrement,
        DecrementWrap,
    };

    /// @brief  stencil faces
    enum class StencilFace : uint8_t
    {
        Front = 0x1,                 
        Back = 0x2,                  
        FrontAndBack = Front | Back, 
    };

    /// @brief Stream for external textures
    enum class StreamType
    {
        Native,   
        Acquired, 
    };

    /// @brief Shader Stage
    enum class ShaderStage : uint8_t
    {
        Vertex = 0,
        Fragment = 1,
        Geometry = 2,
        Compute = 3
    };

    /// @brief  Bitmask for selecting shader stages
    enum class ShaderStageFlags : uint8_t
    {
        None = 0,
        Vertex = 0x1,
        Fragment = 0x2,
        Geometry = 0x4,
        Compute = 0x8,
        NonComputeFlags = Vertex | Fragment | Geometry,
        AllStageFlags = Vertex | Fragment | Geometry | Compute
    };

    /// @brief  The type of technique for stereoscopic rendering
    enum class StereoscopicType : uint8_t
    {
        Instanced,
        Multview,
    };

}

#endif // PLUTO_BACKEND_ENUMS_H