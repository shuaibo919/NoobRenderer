#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/SwapChain.h"

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

        class RenderContext;
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
            friend class GLRenderContext;
            friend class VKRenderContext;

        public:
            virtual ~GraphicsContext() = default;
            virtual void Terminate() = 0;

            RenderAPI GetRenderAPI() { return mRenderAPI; }
            RenderContext *GetRenderContext() const { return mRenderContext; }

            virtual void Init() = 0;
            virtual void Present() = 0;
            virtual float GetGPUMemoryUsed() = 0;
            virtual float GetTotalGPUMemory() = 0;
            virtual void SetMainSwapChain(SwapChain::Properties &&properties) = 0;

            virtual size_t GetMinUniformBufferOffsetAlignment() const = 0;
            virtual bool FlipImGUITexture() const = 0;
            virtual void WaitIdle() const = 0;
            virtual void OnImGui() = 0;

            virtual void BindToDevice() = 0;

            static SharedPtr<GraphicsContext> Create(RenderAPI api);

        protected:
            RenderAPI mRenderAPI{RenderAPI::None};
            RenderContext *mRenderContext{nullptr};

        protected:
            virtual SharedPtr<Shader> CreateShader(void *&&pPropeties) = 0;
            virtual SharedPtr<Pipeline> CreatePipeline(void *&&pPropeties) = 0;
            virtual SharedPtr<SwapChain> CreateSwapChain(void *&&pPropeties) = 0;
            virtual SharedPtr<RenderPass> CreateRenderPass(void *&&pPropeties) = 0;
            virtual SharedPtr<Framebuffer> CreateFrameBuffer(void *&&pPropeties) = 0;
            virtual SharedPtr<IndexBuffer> CreateIndexBuffer(void *&&pPropeties) = 0;
            virtual SharedPtr<VertexBuffer> CreateVertexBuffer(void *&&pPropeties) = 0;
            virtual SharedPtr<UniformBuffer> CreateUniformBuffer(void *&&pPropeties) = 0;
            virtual SharedPtr<CommandBuffer> CreateCommandBuffer(void *&&pPropeties) = 0;
            virtual SharedPtr<DescriptorSet> CreateDescriptorSet(void *&&pPropeties) = 0;
            virtual SharedPtr<Texture> CreateTexture(uint16_t type, void *&&pPropeties) = 0;
            virtual SharedPtr<Texture> CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties) = 0;
        };
    }
}
