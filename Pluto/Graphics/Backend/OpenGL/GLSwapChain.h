#pragma once
#include "Graphics/RHI/SwapChain.h"

namespace pluto
{
    namespace Graphics
    {
        class RenderDevice;

        class GLTexture2D;
        class GLRenderCommand;

        class GLSwapChain : public SwapChain
        {
            friend class GLContext;

        public:
            GLSwapChain(RenderContext *ctx, Properties *&&pProperties);
            ~GLSwapChain();
            void OnResize(uint32_t width, uint32_t height);

        public:
            bool Init(bool vsync) override;
            void Submit(SharedPtr<RenderCommand> command) override;
            SharedPtr<Texture> GetCurrentImage() override;
            SharedPtr<Texture> GetImage(uint32_t index) override;
            uint32_t GetCurrentBufferIndex() const override;
            uint32_t GetCurrentImageIndex() const override;
            SharedPtr<RenderCommand> GetCurrentRenderCommand() override;
            size_t GetSwapChainBufferCount() const override;
            void SetVSync(bool vsync) override;
            void BeginFrame() override {};
            void EndFrame() override {};

        private:
            SharedPtr<GLRenderCommand> mCommand{nullptr};
            uint32_t mCurrentBuffer{0};
        };
    }
}
