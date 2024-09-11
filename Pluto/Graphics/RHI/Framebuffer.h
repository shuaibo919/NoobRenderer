#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class Framebuffer : public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = SharedPtr<Framebuffer>;
            struct Properties
            {
                uint32_t width{0};
                uint32_t height{0};
                uint32_t layer{0};
                uint32_t samples{1};
                int mipIndex{0};
                bool screenUse{false};
                std::vector<SharedPtr<Texture>> attachments;
                std::vector<AttachmentType> attachmentTypes;
                SharedPtr<RenderPass> renderPass;
            };
            struct Builder final : BuildBase<Framebuffer::Properties, Framebuffer>
            {
                Builder() {}
                Framebuffer::Builder &SetBase(uint32_t width, uint32_t height, uint32_t layer);
                Framebuffer::Builder &SetMipIndex(int index);
                Framebuffer::Builder &SetSamples(uint32_t samples);
                Framebuffer::Builder &SetUseScreen(bool use);
                Framebuffer::Builder &SetRenderPass(SharedPtr<RenderPass> &&renderPass);
                Framebuffer::Builder &SetAttachment(SharedPtr<Texture> &&texture, AttachmentType type);
                Framebuffer::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~Framebuffer();

            const Properties &GetProperties() const { return *mProperties; }

            virtual void Validate() {};
            virtual void SetClearColor(const glm::vec4 &color) = 0;

        protected:
            Properties *mProperties;
            Framebuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
