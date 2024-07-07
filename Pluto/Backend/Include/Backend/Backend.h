#ifndef PLUTO_GRAPHICS_CONTEXT_H
#define PLUTO_GRAPHICS_CONTEXT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Backend/BackendEnums.h>

namespace pluto::backend
{
    struct SamplerParams
    {
        SamplerMagFilter filterMag;
        SamplerMinFilter filterMin;
        SamplerWrapMode wrapS;
        SamplerWrapMode wrapT;
        SamplerWrapMode wrapR;
        SamplerCompareMode compareMode;
        SamplerCompareFunc compareFunc;
    };

    struct Attribute
    {
        uint32_t offset = 0;
        uint8_t stride = 0;
        uint8_t buffer = BUFFER_UNUSED;
        ElementType type = ElementType::Byte;
        uint8_t flags = 0x0;
    };

    struct RasterizationState
    {
        using CullingMode = backend::CullingMode;
        using DepthFunc = backend::SamplerCompareFunc;
        using BlendEquation = backend::BlendEquation;
        using BlendFunction = backend::BlendFunction;

        RasterizationState() noexcept
        {
            culling = CullingMode::Back;
            blendEquationRGB = BlendEquation::Add;
            blendEquationAlpha = BlendEquation::Add;
            blendFunctionSrcRGB = BlendFunction::One;
            blendFunctionSrcAlpha = BlendFunction::One;
            blendFunctionDstRGB = BlendFunction::Zero;
            blendFunctionDstAlpha = BlendFunction::Zero;
        }

        bool operator==(RasterizationState rhs) const noexcept { return u == rhs.u; }
        bool operator!=(RasterizationState rhs) const noexcept { return u != rhs.u; }

        void disableBlending() noexcept
        {
            blendEquationRGB = BlendEquation::Add;
            blendEquationAlpha = BlendEquation::Add;
            blendFunctionSrcRGB = BlendFunction::One;
            blendFunctionSrcAlpha = BlendFunction::One;
            blendFunctionDstRGB = BlendFunction::Zero;
            blendFunctionDstAlpha = BlendFunction::Zero;
        }

        bool hasBlending() const noexcept
        {
            return !(blendEquationRGB == BlendEquation::Add &&
                     blendEquationAlpha == BlendEquation::Add &&
                     blendFunctionSrcRGB == BlendFunction::One &&
                     blendFunctionSrcAlpha == BlendFunction::One &&
                     blendFunctionDstRGB == BlendFunction::Zero &&
                     blendFunctionDstAlpha == BlendFunction::Zero);
        }

        union
        {
            struct
            {
                CullingMode culling : 2;                 //  2
                BlendEquation blendEquationRGB : 3;      //  5
                BlendEquation blendEquationAlpha : 3;    //  8
                BlendFunction blendFunctionSrcRGB : 4;   // 12
                BlendFunction blendFunctionSrcAlpha : 4; // 16
                BlendFunction blendFunctionDstRGB : 4;   // 20
                BlendFunction blendFunctionDstAlpha : 4; // 24
                bool depthWrite : 1;                     // 25
                DepthFunc depthFunc : 3;                 // 28
                bool colorWrite : 1;                     // 29
                bool alphaToCoverage : 1;                // 30
                bool inverseFrontFaces : 1;              // 31
                uint8_t padding : 1;                     // 32
            };
            uint32_t u = 0;
        };
    };

    /// @brief All drawing is clipped to the viewport.
    struct Viewport
    {
        int32_t left;
        int32_t bottom;
        uint32_t width;
        uint32_t height;

        int32_t right() const noexcept { return left + int32_t(width); }
        int32_t top() const noexcept { return bottom + int32_t(height); }
    };

    /// @brief Specifies the mapping of the near and far clipping plane to window coordinates.
    struct DepthRange
    {
        float near = 0.0f;
        float far = 1.0f;
    };

    class RHILayer
    {
    };
}

#endif // PLUTO_GRAPHICS_CONTEXT_H