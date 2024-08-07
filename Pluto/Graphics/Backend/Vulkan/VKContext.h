#pragma once
#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        namespace Vulkan
        {
            SharedPtr<Shader> CreateShader(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<Pipeline> CreatePipeline(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<SwapChain> CreateSwapChain(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<RenderPass> CreateRenderPass(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<Framebuffer> CreateFrameBuffer(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<IndexBuffer> CreateIndexBuffer(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<VertexBuffer> CreateVertexBuffer(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<UniformBuffer> CreateUniformBuffer(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<CommandBuffer> CreateCommandBuffer(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<DescriptorSet> CreateDescriptorSet(RenderContext *ctx, void *&&pPropeties);
            SharedPtr<Texture> CreateTexture(uint16_t type, RenderContext *ctx, void *&&pPropeties);
            SharedPtr<Texture> CreateTexture(uint16_t type, const std::string &path, RenderContext *ctx, void *&&pPropeties);
        }
        class VKRenderDevice;
        class VKContext : public GraphicsContext, public std::enable_shared_from_this<VKContext>
        {
            friend class GraphicsContext;
            friend class VKRenderDevice;

        public:
            VKContext();
            ~VKContext();

            size_t GetMinUniformBufferOffsetAlignment() const override { return 256; }
            bool FlipImGUITexture() const override { return true; }
            float GetGPUMemoryUsed() override { return 0.0f; }
            float GetTotalGPUMemory() override { return 0.0f; }

            void WaitIdle() const override {}
            void Present() override;
            void OnImGui() override;
            void Init() override;

            void BindToDevice() override;

            /* Context Methods For Vulkan */
            VkInstance GetVKInstance() const { return mVkInstance; }
            uint32_t GetVKVersion() const { return mVKVersion; }
            VKRenderDevice *GetBasedDevice() const { return mRenderDevice; }

        protected:
            static SharedPtr<GraphicsContext> Create();
            SharedPtr<GraphicsContext> Get() { return shared_from_this(); }

        protected:
            SharedPtr<Shader> CreateShader(void *&&pPropeties) override;
            SharedPtr<Pipeline> CreatePipeline(void *&&pPropeties) override;
            SharedPtr<SwapChain> CreateSwapChain(void *&&pPropeties) override;
            SharedPtr<RenderPass> CreateRenderPass(void *&&pPropeties) override;
            SharedPtr<Framebuffer> CreateFrameBuffer(void *&&pPropeties) override;
            SharedPtr<IndexBuffer> CreateIndexBuffer(void *&&pPropeties) override;
            SharedPtr<VertexBuffer> CreateVertexBuffer(void *&&pPropeties) override;
            SharedPtr<UniformBuffer> CreateUniformBuffer(void *&&pPropeties) override;
            SharedPtr<CommandBuffer> CreateCommandBuffer(void *&&pPropeties) override;
            SharedPtr<DescriptorSet> CreateDescriptorSet(void *&&pPropeties) override;
            SharedPtr<Texture> CreateTexture(uint16_t type, void *&&pPropeties) override;
            SharedPtr<Texture> CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties) override;

        private:
            uint32_t mVKVersion;
            VkInstance mVkInstance;
            VKRenderDevice *mRenderDevice;
            VkDebugReportCallbackEXT mDebugCallback{VK_NULL_HANDLE};

            std::vector<VkLayerProperties> mInstanceLayers;
            std::vector<VkExtensionProperties> mInstanceExtensions;

            std::vector<const char *> mInstanceLayerNames;
            std::vector<const char *> mInstanceExtensionNames;

            bool CheckValidationLayerSupport(std::vector<const char *> &validationLayers);
            bool CheckExtensionSupport(std::vector<const char *> &extensions);
            void SetupDebugMessenger();
        };
    }
}
