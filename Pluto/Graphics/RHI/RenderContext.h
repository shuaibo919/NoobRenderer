#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class RHIBase;
        class SwapChain;
        class RenderContext
        {
        public:
            using Ptr = SharedPtr<RenderContext>;
            RenderContext() = default;
            virtual ~RenderContext() = default;
            virtual void Init() = 0;
            virtual void Begin() = 0;
            virtual void WaitIdle() = 0;
            virtual void OnResize(uint32_t width, uint32_t height) = 0;
            virtual void ClearRenderTarget(const SharedPtr<Texture> &texture, AttachmentType type, const SharedPtr<CommandBuffer> &commandBuffer, glm::vec4 clearColor) = 0;

            virtual void Present() = 0;
            virtual void Present(const SharedPtr<CommandBuffer> &commandBuffer) = 0;
            virtual void BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet) = 0;
            virtual void BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets) = 0;

            virtual const std::string &GetTitle() const = 0;
            virtual void DrawIndexed(const SharedPtr<CommandBuffer> &, DrawType type, uint32_t count, uint32_t start) const = 0;
            virtual void Draw(const SharedPtr<CommandBuffer> &, DrawType type, uint32_t count) const = 0;
            virtual void Dispatch(const SharedPtr<CommandBuffer> &, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) = 0;
            virtual void DrawSplashScreen(const SharedPtr<Texture> &texture) = 0;

            virtual SwapChain *GetSwapChain() = 0;

            virtual uint32_t GetGPUCount() const { return 1; }
            virtual bool SupportsCompute() { return false; }
            virtual RHIFormat GetDepthFormat() { return RHIFormat::Depth32Float; };

        protected:
            std::string mTitle;
        };
    }
}
