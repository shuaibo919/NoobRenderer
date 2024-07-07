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
            GLFramebuffer(GLFramebuffer::Properties *&&pProperties);
            ~GLFramebuffer();

        public:
            void Validate() override;
            void SetClearColour(const glm::vec4 &colour) override;

        public:
            inline uint32_t GetFramebuffer() const { return mHandle; }

            void GenerateFramebuffer();
            void Bind(uint32_t width, uint32_t height) const;
            void Bind() const;
            void UnBind() const;
            void Clear() {}
            unsigned int GetAttachmentPoint(Graphics::RHIFormat format);
            void AddTextureAttachment(RHIFormat format, Texture *texture, uint32_t mipLevel = 0);
            void AddCubeTextureAttachment(RHIFormat format, CubeFace face, TextureCube *texture, uint32_t mipLevel = 0);

            void AddShadowAttachment(Texture *texture);
            void AddTextureLayer(int index, Texture *texture);

        private:
            uint32_t mHandle;
        };
    }
}
