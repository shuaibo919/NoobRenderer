#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class SwapChain;
        class RenderContext
        {
        public:
            using Ptr = std::shared_ptr<RenderContext>;
            RenderContext() = default;
            virtual ~RenderContext() = default;

            virtual void Init() = 0;
            virtual void Begin() = 0;
            virtual void OnResize(uint32_t width, uint32_t height) = 0;
            virtual void ClearRenderTarget(std::shared_ptr<Texture> texture, AttachmentType type, std::shared_ptr<CommandBuffer> commandBuffer, glm::vec4 clearColor) = 0;

            virtual void Present() = 0;
            virtual void Present(std::shared_ptr<CommandBuffer> commandBuffer) = 0;
            virtual void BindDescriptorSets(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<CommandBuffer> commandBuffer, uint32_t dynamicOffset, std::vector<std::shared_ptr<DescriptorSet>> descriptorSets) = 0;

            virtual const std::string &GetTitle() const = 0;
            virtual void DrawIndexed(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, uint32_t start) const = 0;
            virtual void Draw(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, DataType datayType, void *indices) const = 0;
            virtual void Dispatch(std::shared_ptr<CommandBuffer>, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) = 0;
            virtual void DrawSplashScreen(std::shared_ptr<Texture> texture) = 0;

            virtual std::shared_ptr<SwapChain> GetSwapChain() = 0;

            virtual uint32_t GetGPUCount() const { return 1; }
            virtual bool SupportsCompute() { return false; }
            virtual RHIFormat GetDepthFormat() { return RHIFormat::Depth32Float; };

        protected:
            std::string mTitle;
        };
    }
}
