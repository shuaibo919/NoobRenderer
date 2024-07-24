#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"
#define MAX_FRAMES_FLIGHT 3
#define MAX_ELEMENTS_PER_SET 32
namespace pluto
{
    class Window;
    namespace Graphics
    {
        class SwapChain : public RHIBase
        {
        public:
            using Ptr = SharedPtr<SwapChain>;
            struct Properties
            {
                uint16_t width{0};
                uint16_t height{0};
                Window *window;
            };
            struct Builder final : BuildBase<SwapChain::Properties, SwapChain>
            {
                Builder() {}
                SwapChain::Builder &SetBase(uint16_t width, uint16_t height);
                SwapChain::Builder &SetWindow(Window *window);
                SwapChain::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~SwapChain();

        public:
            const Properties &GetProperties() const { return *mProperties; }

        public:
            virtual bool Init(bool vsync) = 0;
            virtual SharedPtr<Texture> GetCurrentImage() = 0;
            virtual SharedPtr<Texture> GetImage(uint32_t index) = 0;
            virtual uint32_t GetCurrentBufferIndex() const = 0;
            virtual uint32_t GetCurrentImageIndex() const = 0;
            virtual size_t GetSwapChainBufferCount() const = 0;
            virtual SharedPtr<CommandBuffer> GetCurrentCommandBuffer() = 0;
            virtual void SetVSync(bool vsync) = 0;

        protected:
            Properties *mProperties;
            SwapChain(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
