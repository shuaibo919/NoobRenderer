#pragma once
#include "Core/Base.hpp"

namespace pluto
{
    namespace Graphics
    {
        class Shader;
        class Texture;
        class Pipeline;
        class SwapChain;
        class RenderPass;
        class Framebuffer;
        class IndexBuffer;
        class VertexBuffer;
        class RenderDevice;
        class UniformBuffer;
        class CommandBuffer;
        class DescriptorSet;

        enum class RenderAPI
        {
            OPENGL = 0,
            VULKAN,
            None, // Unsupported
        };

        class GraphicsContext
        {

            friend class Shader;
            friend class Texture;
            friend class Pipeline;
            friend class SwapChain;
            friend class RenderPass;
            friend class Framebuffer;
            friend class IndexBuffer;
            friend class VertexBuffer;
            friend class RenderDevice;
            friend class UniformBuffer;
            friend class CommandBuffer;
            friend class DescriptorSet;

        public:
            virtual ~GraphicsContext();

            RenderAPI GetRenderAPI() { return mRenderAPI; }

            virtual void Init() = 0;
            virtual void Present() = 0;
            virtual float GetGPUMemoryUsed() = 0;
            virtual float GetTotalGPUMemory() = 0;

            virtual size_t GetMinUniformBufferOffsetAlignment() const = 0;
            virtual bool FlipImGUITexture() const = 0;
            virtual void WaitIdle() const = 0;
            virtual void OnImGui() = 0;

            static std::shared_ptr<GraphicsContext> Create(RenderAPI api, RenderDevice const *pDevice);

        protected:
            RenderAPI mRenderAPI{RenderAPI::None};

        protected:
            virtual std::shared_ptr<Shader> CreateShader(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Pipeline> CreatePipeline(void *&&pPropeties) = 0;
            virtual std::shared_ptr<SwapChain> CreateSwapChain(void *&&pPropeties) = 0;
            virtual std::shared_ptr<RenderPass> CreateRenderPass(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Framebuffer> CreateFrameBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<UniformBuffer> CreateUniformBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<CommandBuffer> CreateCommandBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<DescriptorSet> CreateDescriptorSet(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Texture> CreateTexture(uint16_t type, void *&&pPropeties) = 0;
            virtual std::shared_ptr<Texture> CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties) = 0;
        };
    }
}
