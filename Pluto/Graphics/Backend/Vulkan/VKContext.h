#pragma once
#include "Graphics/RHI/GraphicsContext.h"

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
        class RenderDevice;
        class VKContext : public GraphicsContext
        {
            friend class GraphicsContext;

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

        protected:
            static SharedPtr<GraphicsContext> Create(RenderDevice const *device);

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
        };
    }
}
