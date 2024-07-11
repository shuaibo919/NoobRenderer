#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/RHI/RenderContext.h"

namespace pluto
{
    namespace Graphics
    {
        class GLContext;
        class GLRenderContext : public RenderContext
        {
        public:
            GLRenderContext(GLContext *ctx);
            ~GLRenderContext();
            void Clear(uint32_t buffer);

        public:
            void Init() override;
            void Begin() override;
            void OnResize(uint32_t width, uint32_t height) override;
            void ClearRenderTarget(std::shared_ptr<Texture> texture, AttachmentType type, std::shared_ptr<CommandBuffer> commandBuffer, glm::vec4 clearColor) override;

            void Present() override;
            void Present(std::shared_ptr<CommandBuffer> commandBuffer) override;
            void BindDescriptorSets(std::shared_ptr<Pipeline> pipeline, std::shared_ptr<CommandBuffer> commandBuffer, uint32_t dynamicOffset, std::vector<std::shared_ptr<DescriptorSet>> descriptorSets) override;

            const std::string &GetTitle() const override;
            void DrawIndexed(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, uint32_t start) const override;
            void Draw(std::shared_ptr<CommandBuffer>, DrawType type, uint32_t count, DataType datayType, void *indices) const override;
            void Dispatch(std::shared_ptr<CommandBuffer>, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) override;
            void DrawSplashScreen(std::shared_ptr<Texture> texture) override;
            uint32_t GetGPUCount() const override;
            bool SupportsCompute() override;
            RHIFormat GetDepthFormat() override;

            std::shared_ptr<SwapChain> GetSwapChain() override;

        private:
            GLContext *mContext;
        };
    }
}
