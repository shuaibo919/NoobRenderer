#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class RenderPass
        {
            friend class GraphicsContext;

        public:
            using Ptr = std::shared_ptr<RenderPass>;
            struct Properties
            {
                std::vector<std::shared_ptr<Texture>> attachments;
                std::vector<AttachmentType> attachmentTypes;
                bool clear{true};
                bool swapchainTarget{false};
                int cubeMapIndex{-1};
                int mipIndex{0};
                int samples{1};
                std::shared_ptr<Texture> resolveTexture{nullptr};
            };
            struct Builder final : BuildBase<RenderPass::Properties, RenderPass>
            {
                Builder() {}
                RenderPass::Builder &SetAttachment(std::shared_ptr<Texture> &&texture, AttachmentType type);
                RenderPass::Builder &SetClear(bool clear);
                RenderPass::Builder &SetSwapchainTarget(bool swapchainTarget);
                RenderPass::Builder &SetCubeMapIndex(int index);
                RenderPass::Builder &SetMipIndex(int index);
                RenderPass::Builder &SetSamples(uint32_t samples);
                RenderPass::Builder &SetResolveTexture(std::shared_ptr<Texture> &&texture);
                RenderPass::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~RenderPass();

        public:
            const Properties &GetProperties() const { return *mProperties; }

            static void ClearCache();
            static void DeleteUnusedCache();

            virtual void BeginRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer, float *clearColor, std::shared_ptr<Framebuffer> &frame, SubPassContents contents) const = 0;
            virtual void EndRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer) = 0;

        protected:
            Properties *mProperties;
            RenderPass(Properties *&&pProperties);
        };
    }
}
