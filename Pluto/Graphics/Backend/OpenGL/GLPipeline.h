#pragma once
#include "Graphics/RHI/Pipeline.h"

namespace pluto
{
    namespace Graphics
    {
        class Framebuffer;
        class VertexBuffer;
        class CommandBuffer;
        class GLRenderPass;
        class GLShader;

        class GLPipeline : public Pipeline
        {
        public:
            GLPipeline(RenderContext *ctx, Properties *&&pProperties);
            ~GLPipeline();
            void BindVertexArray(SharedPtr<VertexBuffer> vbo);
            void Preparation();

        public:
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer = 0, uint32_t frame = 0) override;
            void End(const SharedPtr<CommandBuffer> &commandBuffer) override;
            void ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer) override;

        private:
            SharedPtr<GLShader> mShader{nullptr};
            SharedPtr<GLRenderPass> mRenderPass{nullptr};
            std::vector<SharedPtr<Framebuffer>> mFramebuffers;
            uint32_t mVertexArray;
        };
    }
}
