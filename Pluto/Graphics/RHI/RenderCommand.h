#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class RenderCommand : public RHIBase
        {
        public:
            using Ptr = SharedPtr<RenderCommand>;
            struct Properties
            {
                CommandBufferUsageType type;
            };
            struct Builder final : BuildBase<RenderCommand::Properties, RenderCommand>
            {
                Builder() {}
                Builder &SetUsageType(CommandBufferUsageType type);
                RenderCommand::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~RenderCommand();

        public:
            const Properties &GetProperties() const { return *mProperties; }

        public:
            virtual bool Flush() { return true; }
            virtual void Reset() = 0;
            virtual void BeginRecording() = 0;
            virtual void EndRecording() = 0;
            virtual void BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding = 0) = 0;
            virtual void BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet) = 0;
            virtual void BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets) = 0;
            virtual void UpdateViewport(uint32_t width, uint32_t height, bool flipViewport = false) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline) = 0;
            virtual void BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer) = 0;
            virtual void DrawIndexed(DrawType type, uint32_t count, uint32_t start) = 0;
            virtual void Draw(DrawType type, uint32_t count) = 0;
            virtual void Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ) = 0;
            virtual void DrawSplashScreen(const SharedPtr<Texture> &texture) = 0;
            virtual void UnBindPipeline() = 0;

        protected:
            Properties *mProperties;
            RenderCommand(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
