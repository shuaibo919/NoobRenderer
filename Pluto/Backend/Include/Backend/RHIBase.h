#ifndef PLUTO_RHI_BASE_H
#define PLUTO_RHI_BASE_H

#include <Backend/BackendEnums.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>
#include <cstdint>

namespace pluto::backend
{

    struct AcquiredImage;

    struct RHIBase
    {
    };

    struct RHIVertexBufferInfo : public RHIBase
    {
        uint8_t bufferCount{};    //   1
        uint8_t attributeCount{}; //   1
        bool padding[2]{};        //   2
        RHIVertexBufferInfo() noexcept = default;
        RHIVertexBufferInfo(uint8_t bufferCount, uint8_t attributeCount) noexcept
            : bufferCount(bufferCount),
              attributeCount(attributeCount)
        {
        }
    };

    struct RHIVertexBuffer : public RHIBase
    {
        uint32_t vertexCount{};             //   4
        uint8_t bufferObjectsVersion{0xff}; //   1
        bool padding[3]{};                  //   2
        RHIVertexBuffer() noexcept = default;
        explicit RHIVertexBuffer(uint32_t vertextCount) noexcept
            : vertexCount(vertextCount)
        {
        }
    };

    struct RHIBufferObject : public RHIBase
    {
        uint32_t byteCount{};

        RHIBufferObject() noexcept = default;
        explicit RHIBufferObject(uint32_t byteCount) noexcept : byteCount(byteCount) {}
    };

    struct RHIIndexBuffer : public RHIBase
    {
        uint32_t count : 27;
        uint32_t elementSize : 5;

        RHIIndexBuffer() noexcept : count{}, elementSize{} {}
        RHIIndexBuffer(uint8_t elementSize, uint32_t indexCount) noexcept : count(indexCount), elementSize(elementSize)
        {
        }
    };

    struct HwRenderPrimitive : public RHIBase
    {
        PrimitiveType type = PrimitiveType::Triangles;
    };

    struct RHIProgram : public RHIBase
    {
        // utils::CString name;
        // explicit RHIProgram(utils::CString name) noexcept : name(std::move(name)) {}
        // RHIProgram() noexcept = default;
    };

    struct RHISamplerGroup : public RHIBase
    {
        RHISamplerGroup() noexcept = default;
    };

    struct RHITexture : public RHIBase
    {
        uint32_t width{};
        uint32_t height{};
        uint32_t depth{};
        SamplerType target{};
        uint8_t levels : 4;  // This allows up to 15 levels (max texture size of 32768 x 32768)
        uint8_t samples : 4; // Sample count per pixel (should always be a power of 2)
        TextureFormat format{};
        uint8_t reserved0 = 0;
        TextureUsage usage{};
        uint16_t reserved1 = 0;
        RHIStream *hwStream = nullptr;

        RHITexture() noexcept : levels{}, samples{} {}
        RHITexture(backend::SamplerType target, uint8_t levels, uint8_t samples,
                   uint32_t width, uint32_t height, uint32_t depth, TextureFormat fmt, TextureUsage usage) noexcept
            : width(width), height(height), depth(depth),
              target(target), levels(levels), samples(samples), format(fmt), usage(usage) {}
    };

    struct RHIRenderTarget : public RHIBase
    {
        uint32_t width{};
        uint32_t height{};
        RHIRenderTarget() noexcept = default;
        RHIRenderTarget(uint32_t w, uint32_t h) : width(w), height(h) {}
    };

    struct RHIFence : public RHIBase
    {
        // Platform::Fence *fence = nullptr;
    };

    struct RHISwapChain : public RHIBase
    {
        // Platform::SwapChain *swapChain = nullptr;
    };

    struct RHIStream : public RHIBase
    {
        // Platform::Stream *stream = nullptr;
        // StreamType streamType = StreamType::ACQUIRED;
        // uint32_t width{};
        // uint32_t height{};

        // RHIStream() noexcept = default;
        // explicit RHIStream(Platform::Stream *stream) noexcept
        //     : stream(stream), streamType(StreamType::NATIVE)
        // {
        // }
    };

} // namespace pluto::backend

#endif // PLUTO_RHI_BASE_H
