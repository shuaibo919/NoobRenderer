#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class RenderPass : public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = SharedPtr<RenderPass>;
            struct Properties
            {
                std::vector<SharedPtr<Texture>> attachments;
                std::vector<AttachmentType> attachmentTypes;
                bool clear{true};
                bool swapchainTarget{false};
                int cubeMapIndex{-1};
                int mipIndex{0};
                int samples{1};
                SharedPtr<Texture> resolveTexture{nullptr};
            };
            struct Builder final : BuildBase<RenderPass::Properties, RenderPass>
            {
                Builder() {}
                RenderPass::Builder &SetAttachment(SharedPtr<Texture> &&texture, AttachmentType type);
                RenderPass::Builder &SetClear(bool clear);
                RenderPass::Builder &SetSwapchainTarget(bool swapchainTarget);
                RenderPass::Builder &SetCubeMapIndex(int index);
                RenderPass::Builder &SetMipIndex(int index);
                RenderPass::Builder &SetSamples(uint32_t samples);
                RenderPass::Builder &SetResolveTexture(SharedPtr<Texture> &&texture);
                RenderPass::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~RenderPass();

        public:
            const Properties &GetProperties() const { return *mProperties; }

            static void ClearCache();
            static void DeleteUnusedCache();

            virtual void BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const = 0;
            virtual void EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer) = 0;

        protected:
            Properties *mProperties;
            RenderPass(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
