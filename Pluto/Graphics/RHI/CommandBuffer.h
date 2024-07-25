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
            
            /* move RenderContext cap to here */
            virtual void Present(const SharedPtr<CommandBuffer> &commandBuffer) = 0;
            virtual void BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet) = 0;
            virtual void BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets) = 0;
            virtual void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport = false) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer) = 0;
            virtual void DrawIndexed(DrawType type, uint32_t count, uint32_t start) const = 0;
            virtual void Draw(DrawType type, uint32_t count) const = 0;
            virtual void Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) = 0;
            virtual void DrawSplashScreen(const SharedPtr<Texture> &texture) = 0;
            virtual void UnBindPipeline() = 0;
            virtual void EndCurrentRenderPass() = 0;

        protected:
            Properties *mProperties;
            CommandBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
