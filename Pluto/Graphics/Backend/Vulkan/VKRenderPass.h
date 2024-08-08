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
            void BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const override;
            void EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer) override;

        public:
            /* TODO */
            VkRenderPass GetHandle() const { return VK_NULL_HANDLE; }
        };

    }
}
