#pragma once
#include "Graphics/RHI/SwapChain.h"

#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKTexture2D;
        class VKSemaphore;
        class VKCommandPool;
        class VKRenderDevice;
        class VKCommandBuffer;
        struct FrameData
        {
            SharedPtr<VKSemaphore> ImageAcquireSemaphore;
            SharedPtr<VKCommandPool> CommandPool;
            SharedPtr<VKCommandBuffer> MainCommandBuffer;
        };
        class VKSwapChain : public SwapChain
        {
            friend class VKContext;

        public:
            VKSwapChain(RenderContext *ctx, Properties *&&pProperties);
            ~VKSwapChain();
            void OnResize(uint32_t width, uint32_t height);

        public:
            bool Init(bool vsync, const SharedPtr<RenderDevice> &pDevice = nullptr) override;

            SharedPtr<Texture> GetCurrentImage() override;
            SharedPtr<Texture> GetImage(uint32_t index) override;
            uint32_t GetCurrentBufferIndex() const override;
            uint32_t GetCurrentImageIndex() const override;
            SharedPtr<CommandBuffer> GetCurrentCommandBuffer() override;
            size_t GetSwapChainBufferCount() const override;
            void SetVSync(bool vsync) override;

        public:
            void PrepareFrameData();

        private:
            SharedPtr<VKRenderDevice> mBasedDevice;
            FrameData mFrames[MaxFlightFrames];

            void FindImageFormatAndColourSpace();

            std::vector<SharedPtr<Texture>> mSwapChainBuffers;

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
