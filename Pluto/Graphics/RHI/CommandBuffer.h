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
            using Ptr = std::shared_ptr<CommandBuffer>;
            struct Properties
            {
            };
            struct Builder final : BuildBase<CommandBuffer::Properties, CommandBuffer>
            {
                Builder() {}
                CommandBuffer::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~CommandBuffer();

        public:
            const Properties &GetProperties() const { return *mProperties; }

            virtual bool Flush() { return true; }
            virtual void Submit() {}

        public:
            virtual bool Init(bool primary = true) = 0;
            virtual void Unload() = 0;
            virtual void BeginRecording() = 0;
            virtual void BeginRecordingSecondary(std::shared_ptr<RenderPass> &renderPass, std::shared_ptr<Framebuffer> &framebuffer) = 0;
            virtual void EndRecording() = 0;
            virtual void ExecuteSecondary(std::shared_ptr<CommandBuffer> &primaryCmdBuffer) = 0;
            virtual void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport = false) = 0;
            virtual void BindPipeline(std::shared_ptr<Pipeline> &pipeline) = 0;
            virtual void BindPipeline(std::shared_ptr<Pipeline> &pipeline, uint32_t layer) = 0;
            virtual void UnBindPipeline() = 0;
            virtual void EndCurrentRenderPass() = 0;

        protected:
            Properties *mProperties;
            CommandBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
