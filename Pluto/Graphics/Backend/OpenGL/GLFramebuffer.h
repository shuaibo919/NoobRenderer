#pragma once
#include "Graphics/RHI/Framebuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLFramebuffer : public Framebuffer
        {
            friend class GLContext;

        public:
            GLFramebuffer(RenderContext *ctx, GLFramebuffer::Properties *&&pProperties);
            ~GLFramebuffer();

        public:
            void Validate() override;
            void SetClearColor(const glm::vec4 &color) override;

        public:
            inline uint32_t GetFramebuffer() const { return mHandle; }

            void Bind() const;
            void UnBind() const;
            void Clear() {}
            unsigned int GetAttachmentPoint(Graphics::RHIFormat format);
            void AddTextureAttachment(std::shared_ptr<Texture> &texture, uint32_t mipLevel = 0);
            void AddCubeTextureAttachment(CubeFace face, std::shared_ptr<TextureCube> &texture, uint32_t mipLevel = 0);
            void AddDepthAttachment(std::shared_ptr<Texture> &texture);
            void AddTextureLayer(std::shared_ptr<Texture> &texture, int index);

        private:
            uint32_t mHandle;
            uint32_t mColorAttachmentCount;
            glm::vec4 mClearColor;
            std::vector<uint32_t> mAttachmentChannels;
        };
    }
}
