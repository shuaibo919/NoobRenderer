#pragma once
#include "Graphics/RHI/SwapChain.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKContext;
        class VKTexture2D;
        class VKSemaphore;
        class VKCommandPool;
        class VKRenderDevice;
        class VKCommandBuffer;
        class VKRenderCommand;
        struct FrameData
        {
            SharedPtr<VKSemaphore> ImageAcquireSemaphore;
            SharedPtr<VKCommandPool> CommandPool;
            SharedPtr<VKCommandBuffer> CachedCommandBuffer;
        };
        class VKSwapChain : public SwapChain
        {
            friend class VKContext;

        public:
            VKSwapChain(RenderContext *ctx, Properties *&&pProperties);
            ~VKSwapChain();
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
            void BeginFrame() override;
            void EndFrame() override;

        public:
            void PrepareFrameData();
            FrameData &GetCurrentFrameData();
            VkFormat GetFormat() const { return mColourFormat; }
            void Present(const std::vector<VkSemaphore> &semaphore);
            VkCommandPool GetFrameCommandPool(uint32_t index) const;

        private:
            VKRenderDevice *mBasedDevice;
            FrameData mFrames[MaxFlightFrames];

            void FindImageFormatAndColourSpace();
            void AcquireNextImage();

            SharedPtr<VKRenderCommand> mCurrentRenderCommand{nullptr};
            std::vector<SharedPtr<VKTexture2D>> mSwapChainBuffers;

            uint32_t mCurrentBuffer = 0;
            uint32_t mAcquireImageIndex = 0;
            uint32_t mQueueNodeIndex = UINT32_MAX;
            uint32_t mSwapChainBufferCount;
            bool mVSyncEnabled = false;

            VkSwapchainKHR mSwapChain;
            VkSwapchainKHR mOldSwapChain;
            VkSurfaceKHR mSurface;
            VkFormat mColourFormat;
            VkColorSpaceKHR mColourSpace;
        };
    }
}
