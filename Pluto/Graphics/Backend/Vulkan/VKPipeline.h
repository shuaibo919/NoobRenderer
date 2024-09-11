#pragma once
#include "Graphics/RHI/Pipeline.h"
#include "Graphics/Backend/Vulkan/Vk.h"
namespace pluto
{
    namespace Graphics
    {
        class VKShader;
        class VKRenderPass;
        class VKPipeline : public Pipeline, public VKObjectManageByContext
        {
        public:
            VKPipeline(RenderContext *ctx, Properties *&&pProperties);
            ~VKPipeline();

        public:
            void DestroyImplementation() override;
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer = 0) override;
            void End(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer) override;

        private:
            void TransitionAttachments();
            void PrepareFramebuffer();

        public:
            VkPipeline GetHandle() const;
            bool IsComputePipeline() const;
            VkPipelineLayout GetPipelineLayout() const;

        private:
            VkPipeline mPipeline;
            SharedPtr<VKShader> mShader{nullptr};
            SharedPtr<VKRenderPass> mRenderPass{nullptr};
            std::vector<SharedPtr<Framebuffer>> mFramebuffers;
        };
    }
}
