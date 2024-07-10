#pragma once
#include "Graphics/RHI/SwapChain.h"

namespace pluto
{
    namespace Graphics
    {
        class GLCommandBuffer;
        class GLTexture2D;

        class GLSwapChain : public SwapChain
        {
            friend class GLContext;

        public:
            GLSwapChain(RenderContext *ctx, Properties *&&pProperties);
            ~GLSwapChain();
            void OnResize(uint32_t width, uint32_t height);

        public:
            bool Init(bool vsync) override;

            std::shared_ptr<Texture> GetCurrentImage() override;
            std::shared_ptr<Texture> GetImage(uint32_t index) override;
            uint32_t GetCurrentBufferIndex() const override;
            uint32_t GetCurrentImageIndex() const override;
            std::shared_ptr<CommandBuffer> GetCurrentCommandBuffer() override;
            size_t GetSwapChainBufferCount() const override;
            void SetVSync(bool vsync) override;

        private:
            std::shared_ptr<GLCommandBuffer> mCommand{nullptr};
            uint32_t mCurrentBuffer{0};
        };
    }
}
