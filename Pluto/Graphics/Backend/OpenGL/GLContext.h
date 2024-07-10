#pragma once
#include "Graphics/RHI/GraphicsContext.h"

namespace pluto
{
    namespace Graphics
    {
        namespace OpenGL
        {
            std::shared_ptr<Shader> CreateShader(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<Pipeline> CreatePipeline(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<SwapChain> CreateSwapChain(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<RenderPass> CreateRenderPass(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<Framebuffer> CreateFrameBuffer(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<IndexBuffer> CreateIndexBuffer(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<VertexBuffer> CreateVertexBuffer(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<UniformBuffer> CreateUniformBuffer(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<CommandBuffer> CreateCommandBuffer(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<DescriptorSet> CreateDescriptorSet(RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<Texture> CreateTexture(uint16_t type, RenderContext *ctx, void *&&pPropeties);
            std::shared_ptr<Texture> CreateTexture(uint16_t type, const std::string &path, RenderContext *ctx, void *&&pPropeties);
        }
        class GLContext : public GraphicsContext
        {
            friend class GraphicsContext;

        public:
            GLContext();
            ~GLContext();

            size_t GetMinUniformBufferOffsetAlignment() const override { return 256; }
            bool FlipImGUITexture() const override { return true; }
            float GetGPUMemoryUsed() override { return 0.0f; }
            float GetTotalGPUMemory() override { return 0.0f; }

            void WaitIdle() const override {}
            void Present() override;
            void OnImGui() override;
            void Init() override;

            static int LoadGladProc(void *proc);

        protected:
            static std::shared_ptr<GraphicsContext> Create();

        protected:
            std::shared_ptr<Shader> CreateShader(void *&&pPropeties) override;
            std::shared_ptr<Pipeline> CreatePipeline(void *&&pPropeties) override;
            std::shared_ptr<SwapChain> CreateSwapChain(void *&&pPropeties) override;
            std::shared_ptr<RenderPass> CreateRenderPass(void *&&pPropeties) override;
            std::shared_ptr<Framebuffer> CreateFrameBuffer(void *&&pPropeties) override;
            std::shared_ptr<IndexBuffer> CreateIndexBuffer(void *&&pPropeties) override;
            std::shared_ptr<VertexBuffer> CreateVertexBuffer(void *&&pPropeties) override;
            std::shared_ptr<UniformBuffer> CreateUniformBuffer(void *&&pPropeties) override;
            std::shared_ptr<CommandBuffer> CreateCommandBuffer(void *&&pPropeties) override;
            std::shared_ptr<DescriptorSet> CreateDescriptorSet(void *&&pPropeties) override;
            std::shared_ptr<Texture> CreateTexture(uint16_t type, void *&&pPropeties) override;
            std::shared_ptr<Texture> CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties) override;
        };
    }
}
