#pragma once
#include "Graphics/RHI/SwapChain.h"

namespace pluto
{
    namespace Graphics
    {
        class VKCommandBuffer;
        class VKTexture2D;

        class VKSwapChain : public SwapChain
        {
            friend class VKContext;

        public:
            VKSwapChain(RenderContext *ctx, Properties *&&pProperties);
            VKSwapChain();
            void OnResize(uint32_t width, uint32_t height);

        public:
            bool Init(bool vsync) override;

            SharedPtr<Texture> GetCurrentImage() override;
            SharedPtr<Texture> GetImage(uint32_t index) override;
            uint32_t GetCurrentBufferIndex() const override;
            uint32_t GetCurrentImageIndex() const override;
            SharedPtr<CommandBuffer> GetCurrentCommandBuffer() override;
            size_t GetSwapChainBufferCount() const override;
            void SetVSync(bool vsync) override;
        };
    }
}
