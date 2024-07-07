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
            GLSwapChain(Properties *&&pProperties);
            ~GLSwapChain();
            void OnResize(uint32_t width, uint32_t height);

        public:
            // bool Init(bool vsync, Window *window) override { return Init(vsync); };
            bool Init(bool vsync) override;

            std::shared_ptr<Texture> GetCurrentImage() override;
            std::shared_ptr<Texture> GetImage(uint32_t index) override { return nullptr; };
            uint32_t GetCurrentBufferIndex() const override;
            uint32_t GetCurrentImageIndex() const override { return 0; };
            std::shared_ptr<CommandBuffer> GetCurrentCommandBuffer() override;
            size_t GetSwapChainBufferCount() const override;
            void SetVSync(bool vsync) override {}

        private:
            std::vector<std::shared_ptr<GLTexture2D>> mBuffers;
            std::shared_ptr<GLCommandBuffer> mCommand{nullptr};
            uint32_t mCurrentBuffer{0};
        };
    }
}
