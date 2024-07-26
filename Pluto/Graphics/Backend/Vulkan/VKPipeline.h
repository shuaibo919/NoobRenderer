#pragma once
#include "Graphics/RHI/Pipeline.h"

namespace pluto
{
    namespace Graphics
    {
        class Framebuffer;
        class VertexBuffer;
        class CommandBuffer;

        class VKPipeline : public Pipeline
        {
        public:
            VKPipeline(RenderContext *ctx, Properties *&&pProperties);
            ~VKPipeline();
            void BindVertexArray(SharedPtr<VertexBuffer> vbo);
            void Preparation();

        public:
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer = 0) override;
            void End(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer) override;
        };
    }
}
