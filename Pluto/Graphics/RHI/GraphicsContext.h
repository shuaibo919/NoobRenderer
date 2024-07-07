#pragma once
#include "Core/Base.hpp"

namespace pluto
{
    namespace Graphics
    {
        class VertexBuffer;
        class IndexBuffer;
        class Texture;
        class RenderPass;
        class CommandBuffer;
        class Framebuffer;
        class RenderDevice;
        class SwapChain;
        class Pipeline;

        enum class RenderAPI
        {
            OPENGL = 0,
            VULKAN,
            NONE, // Unsupported
        };

        class GraphicsContext
        {
            friend class VertexBuffer;
            friend class Texture;
            friend class RenderPass;
            friend class CommandBuffer;
            friend class Framebuffer;
            friend class IndexBuffer;
            friend class RenderDevice;
            friend class SwapChain;
            friend class Pipeline;

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
            RenderAPI mRenderAPI{RenderAPI::NONE};

        protected:
            virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<RenderPass> CreateRenderPass(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Framebuffer> CreateFrameBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<CommandBuffer> CreateCommandBuffer(void *&&pPropeties) = 0;
            virtual std::shared_ptr<SwapChain> CreateSwapChain(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Pipeline> CreatePipeline(void *&&pPropeties) = 0;
            virtual std::shared_ptr<Texture> CreateTexture(uint16_t type, void *&&pPropeties) = 0;
            virtual std::shared_ptr<Texture> CreateTexture(uint16_t type, const std::string &path, void *&&pPropetie) = 0;
        };
    }
}
