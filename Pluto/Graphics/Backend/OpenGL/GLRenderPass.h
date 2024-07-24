#pragma once

#include "Graphics/RHI/RenderPass.h"

namespace pluto
{
    namespace Graphics
    {
        class GLRenderPass : public RenderPass
        {
            friend class GLContext;

        public:
            GLRenderPass(RenderContext *ctx, GLRenderPass::Properties *&&pProperties);
            ~GLRenderPass();

        public:
            void BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const override;
            void EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer) override;
        };

    }
}
