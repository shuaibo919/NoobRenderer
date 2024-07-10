#pragma once
#include "Graphics/RHI/CommandBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLCommandBuffer : public CommandBuffer
        {
        public:
            GLCommandBuffer(RenderContext *ctx, Properties *&&pProperties);
            ~GLCommandBuffer();

        public:
            bool Init(bool primary) override;
            void Unload() override;
            void BeginRecording() override;
            void BeginRecordingSecondary(std::shared_ptr<RenderPass> &renderPass, std::shared_ptr<Framebuffer> &framebuffer) override;
            void EndRecording() override;
            void ExecuteSecondary(std::shared_ptr<CommandBuffer> &primaryCmdBuffer) override;

            void BindPipeline(std::shared_ptr<Pipeline> &pipeline) override;
            void BindPipeline(std::shared_ptr<Pipeline> &pipeline, uint32_t layer) override;
            void UnBindPipeline() override;
            void EndCurrentRenderPass() override {};
            void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport) override {};

            GLCommandBuffer::Ptr Get();

        private:
            bool primary;
            uint32_t mBoundPipelineLayer = 0;
            std::shared_ptr<Pipeline> mBoundPipeline{nullptr};
        };
    }
}
