#pragma once
#include "glad/glad.h"
#include "KHR/khrplatform.h"

// Graphics Type
namespace gtype
{

    enum class TexType : GLenum
    {
        Tex2D = GL_TEXTURE_2D,
        Tex2DArray = GL_TEXTURE_2D_ARRAY,
        Tex2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
        TexCube = GL_TEXTURE_CUBE_MAP
    };
    enum class Format : GLint
    {
        NONE = 0,
        RGBA = GL_RGBA,
        RGB = GL_RGB,
        RG = GL_RG,
        RED = GL_RED,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        RGBA32F = GL_RGBA32F,
        RGB32F = GL_RGB32F,
        RG32F = GL_RG32F,
        R32F = GL_R32F,
        RGBA16F = GL_RGBA16F,
        RGB16F = GL_RGB16F,
        RG16F = GL_RG16F,
        R16F = GL_R16F,
    };
    enum class DataType : GLenum
    {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        FLOAT = GL_FLOAT
    };
    enum class TexPara : GLint
    {
        Repeat = GL_REPEAT,
        ClampEdge = GL_CLAMP_TO_EDGE,
        ClampBorder = GL_CLAMP_TO_BORDER,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
        NearsetMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    };

    enum class TexParaType : GLenum
    {
        MinFilter = GL_TEXTURE_MIN_FILTER,
        MagFilter = GL_TEXTURE_MAG_FILTER,
        WrapS = GL_TEXTURE_WRAP_S,
        WrapT = GL_TEXTURE_WRAP_T,
        WrapR = GL_TEXTURE_WRAP_R,
        BorderColor = GL_TEXTURE_BORDER_COLOR
    };

    enum class Capability
    {
        Blend = GL_BLEND,                               // If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
        ClipDistance = GL_COLOR_LOGIC_OP,               // If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values. See glLogicOp.
        CullFace = GL_CULL_FACE,                        // If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
        DepthClamp = GL_DEPTH_CLAMP,                    // If enabled, the −wc≤zc≤wc plane equation is ignored by view volume clipping (effectively, there is no near or far plane clipping). See glDepthRange.
        DepthTest = GL_DEPTH_TEST,                      // If enabled, write depth data. See glDepthFunc.
        MultiSample = GL_MULTISAMPLE,                   // If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
        StencilTest = GL_STENCIL_TEST,                  // If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
        CubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS, // If enabled, cubemap textures are sampled such that when linearly sampling from the border between two adjacent faces, texels from both faces are used to generate the final sample value. When disabled, texels from only a single face are used to construct the final sample value.
        PolygonOffsetFill = GL_POLYGON_OFFSET_FILL,     // If enabled, and if the polygon is rendered in GL_FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
        PolygonOffsetLine = GL_POLYGON_OFFSET_LINE,
        PolygonOffsetPoint = GL_POLYGON_OFFSET_POINT,                  // If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, if the polygon is rendered in GL_POINT mode. See glPolygonOffset.
        PoltgonSmooth = GL_POLYGON_SMOOTH,                             // If enabled, draw polygons with proper filtering. Otherwise, draw aliased polygons. For correct antialiased polygons, an alpha buffer is needed and the polygons must be sorted front to back.
        PrimitiveRestart = GL_PRIMITIVE_RESTART,                       // Enables primitive restarting. If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the primitive restart index. See glPrimitiveRestartIndex.
        PrimitiveRestartFixedIndex = GL_PRIMITIVE_RESTART_FIXED_INDEX, // Enables primitive restarting with a fixed index. If enabled, any one of the draw commands which transfers a set of generic attribute array elements to the GL will restart the primitive when the index of the vertex is equal to the fixed primitive index for the specified index type. The fixed index is equal to 2n−1 where n is equal to 8 for GL_UNSIGNED_BYTE, 16 for GL_UNSIGNED_SHORT and 32 for GL_UNSIGNED_INT.
        RasterizerDiscard = GL_RASTERIZER_DISCARD,                     // If enabled, primitives are discarded after the optional transform feedback stage, but before rasterization. Furthermore, when enabled, glClear, glClearBufferData, glClearBufferSubData, glClearTexImage, and glClearTexSubImage are ignored.
        SampleAlphaToCoverage = GL_SAMPLE_ALPHA_TO_COVERAGE,           // If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value.
        SampleAlphaToOne = GL_SAMPLE_ALPHA_TO_ONE,                     // If enabled, each sample alpha value is replaced by the maximum representable alpha value.
        SampleCoverage = GL_SAMPLE_COVERAGE,                           // If enabled, the fragment's coverage is ANDed with the temporary coverage value. If GL_SAMPLE_COVERAGE_INVERT is set to GL_TRUE, invert the coverage value. See glSampleCoverage.
        SampleShading = GL_SAMPLE_SHADING,                             // If enabled, the active fragment shader is run once for each covered sample, or at fraction of this rate as determined by the current value of GL_MIN_SAMPLE_SHADING_VALUE. See glMinSampleShading.
        SampleMask = GL_SAMPLE_MASK,                                   // If enabled, the sample coverage mask generated for a fragment during rasterization will be ANDed with the value of GL_SAMPLE_MASK_VALUE before shading occurs. See glSampleMaski.
        ScissorTest = GL_SCISSOR_TEST,                                 // If enabled, discard fragments that are outside the scissor rectangle. See glScissor.
        ProgramPointSize = GL_PROGRAM_POINT_SIZE                       // If enabled and a vertex or geometry shader is active, then the derived point size is taken from the (potentially clipped) shader builtin gl_PointSize and clamped to the implementation-dependent point size range.
    };
    enum CapabilityBufferBit : GLbitfield
    {
        Depth = GL_DEPTH_BUFFER_BIT,
        Stencil = GL_STENCIL_BUFFER_BIT,
        Color = GL_COLOR_BUFFER_BIT
    };

}

// Graphics Api
namespace gapi
{
    // Todo:
    class GraphicsState
    {
    private:
        // std::vector<std::pair<gtype::Capability, bool>> capabilities;

    public:
    };
    // Todo：Encapsulating opengl functions
}