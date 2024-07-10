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
            void BindVertexArray(std::shared_ptr<VertexBuffer> vbo);
            void Preparation();

        public:
            void Bind(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t layer) override;
            void End(std::shared_ptr<CommandBuffer> commandBuffer) override;
            void ClearRenderTargets(std::shared_ptr<CommandBuffer> commandBuffer) override;

        private:
            std::shared_ptr<GLShader> mShader{nullptr};
            std::shared_ptr<GLRenderPass> mRenderPass{nullptr};
            std::vector<std::shared_ptr<Framebuffer>> mFramebuffers;
            uint32_t mVertexArray;
        };
    }
}
