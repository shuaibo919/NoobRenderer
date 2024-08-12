#pragma once
#include "Graphics/RHI/Pipeline.h"
#include "Graphics/Backend/Vulkan/Vk.h"
namespace pluto
{
    namespace Graphics
    {
        class VKShader;
        class VKRenderPass;
        class VKPipeline : public Pipeline
        {
        public:
            VKPipeline(RenderContext *ctx, Properties *&&pProperties);
            ~VKPipeline();

        public:
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer = 0) override;
            void End(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer) override;

        private:
            void TransitionAttachments();
            void PrepareFramebuffer();

        private:
            VkPipeline mPipeline;
            SharedPtr<VKShader> mShader{nullptr};
            SharedPtr<VKRenderPass> mRenderPass{nullptr};
            std::vector<SharedPtr<Framebuffer>> mFramebuffers;
        };
    }
}
