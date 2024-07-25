#pragma once
#include "Graphics/RHI/CommandBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        struct GLCommandCall
        {
            std::function<void(void)> call;
            GLCommandCall(std::function<void(void)> &&_call) : call(std::forward<std::function<void(void)>>(_call)) {}
        };
        class GLCommandBuffer : public CommandBuffer
        {
        public:
            GLCommandBuffer(RenderContext *ctx, Properties *&&pProperties);
            ~GLCommandBuffer();

        public:
            void Submit() override;
            bool Init(bool primary) override;
            void Unload() override;
            void BeginRecording() override;
            void BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer) override;
            void EndRecording() override;
            void ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer) override;

            /* move RenderContext cap to here */


        public:
            GLCommandBuffer::Ptr Get();
            void EmulateRecording(GLCommandCall &&_call);

        private:
            bool primary;
            bool mRecording;
            uint32_t mBoundPipelineLayer = 0;
            std::vector<GLCommandCall> mCmds;
            SharedPtr<Pipeline> mBoundPipeline{nullptr};
        };
    }
}
