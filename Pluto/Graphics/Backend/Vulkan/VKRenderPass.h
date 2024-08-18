#pragma once
#include "Graphics/RHI/RenderPass.h"
#include "Graphics/Backend/Vulkan/Vk.h"
namespace pluto
{
    namespace Graphics
    {
        class VKRenderPass : public RenderPass
        {
            friend class VKContext;

        public:
            VKRenderPass(RenderContext *ctx, VKRenderPass::Properties *&&pProperties);
            ~VKRenderPass();

        public:
            void DestroyImplementation() override;
            void BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const override;
            void EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer) override;

        public:
            VkRenderPass GetHandle() const { return mRenderPass; }
            uint32_t GetColorAttachments() const { return mColorAttachments; }
            uint32_t GetDepthAttachments() const { return mDepthAttachments; }
            uint32_t GetTotalAttachments() const { return static_cast<uint32_t>(mProperties->attachments.size()); }

        private:
            bool mDepthOnly;
            bool mClearDepth;
            uint32_t mColorAttachments;
            uint32_t mDepthAttachments;
            VkRenderPass mRenderPass;
            VkClearValue *mClearValue;
            uint32_t mClearValueCount;
        };

    }
}
