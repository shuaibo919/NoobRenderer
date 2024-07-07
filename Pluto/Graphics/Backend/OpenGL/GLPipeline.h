#pragma once
#include "Graphics/RHI/Pipeline.h"
#include "Graphics/RHI/Framebuffer.h"
// #include "Graphics/RHI/BufferLayout.h"

namespace pluto
{
    namespace Graphics
    {
        class GLRenderPass;
        class CommandBuffer;
        class RenderPass;

        class GLPipeline : public Pipeline
        {
        public:
            GLPipeline(Properties *&&pProperties);
            ~GLPipeline();
            void BindVertexArray();
            void CreateFramebuffers();

        public:
            void Bind(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t layer) override;
            void End(std::shared_ptr<CommandBuffer> commandBuffer) override;
            void ClearRenderTargets(std::shared_ptr<CommandBuffer> commandBuffer) override;

        private:
            std::shared_ptr<RenderPass> mRenderPass;
            std::vector<std::shared_ptr<Framebuffer>> mFramebuffers;
            uint32_t mVertexArray = -1;
        };
    }
}
