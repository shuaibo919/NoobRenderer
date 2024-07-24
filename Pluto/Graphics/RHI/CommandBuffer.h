#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class CommandBuffer : public std::enable_shared_from_this<CommandBuffer>, public RHIBase
        {
        public:
            using Ptr = SharedPtr<CommandBuffer>;
            struct Properties
            {
            };
            struct Builder final : BuildBase<CommandBuffer::Properties, CommandBuffer>
            {
                Builder() {}
                CommandBuffer::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~CommandBuffer();

        public:
            const Properties &GetProperties() const { return *mProperties; }

            virtual bool Flush() { return true; }
            virtual void Submit() = 0;

        public:
            virtual bool Init(bool primary = true) = 0;
            virtual void Unload() = 0;
            virtual void BeginRecording() = 0;
            virtual void BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer) = 0;
            virtual void EndRecording() = 0;
            virtual void ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer) = 0;
            virtual void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport = false) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer) = 0;
            virtual void UnBindPipeline() = 0;
            virtual void EndCurrentRenderPass() = 0;

        protected:
            Properties *mProperties;
            CommandBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
