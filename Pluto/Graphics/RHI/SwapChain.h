#pragma once
#include "Graphics/RHI/Declarations.h"
#define MAX_FRAMES_FLIGHT 3
#define MAX_ELEMENTS_PER_SET 32
namespace pluto
{
    class Window;
    namespace Graphics
    {
        class SwapChain
        {
        public:
            using Ptr = std::shared_ptr<SwapChain>;
            struct Properties
            {
                uint16_t width{0};
                uint16_t height{0};
                std::shared_ptr<Window> window;
            };
            struct Builder final : BuildBase<SwapChain::Properties, SwapChain>
            {
                Builder() {}
                SwapChain::Builder &SetBase(uint16_t width, uint16_t height);
                SwapChain::Builder &SetWindow(std::shared_ptr<Window> &Window);
                SwapChain::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~SwapChain();

        public:
            const Properties &GetProperties() const { return *mProperties; }

        public:
            virtual bool Init(bool vsync) = 0;
            virtual std::shared_ptr<Texture> GetCurrentImage() = 0;
            virtual std::shared_ptr<Texture> GetImage(uint32_t index) = 0;
            virtual uint32_t GetCurrentBufferIndex() const = 0;
            virtual uint32_t GetCurrentImageIndex() const = 0;
            virtual size_t GetSwapChainBufferCount() const = 0;
            virtual std::shared_ptr<CommandBuffer> GetCurrentCommandBuffer() = 0;
            virtual void SetVSync(bool vsync) = 0;

        protected:
            Properties *mProperties;
            SwapChain(Properties *&&pProperties);
        };
    }
}
