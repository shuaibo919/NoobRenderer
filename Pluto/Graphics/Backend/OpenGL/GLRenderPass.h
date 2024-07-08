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
            GLRenderPass(GLRenderPass::Properties *&&pProperties);
            ~GLRenderPass();

        public:
            void BeginRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer, float *clearColor, std::shared_ptr<Framebuffer> &frame, SubPassContents contents) const override;
            void EndRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer) override;
        };

    }
}
