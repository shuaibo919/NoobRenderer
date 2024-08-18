#pragma once
#include "Graphics/RHI/Framebuffer.h"

#include "Graphics/Backend/Vulkan/Vk.h"
namespace pluto
{
    namespace Graphics
    {
        class VKFramebuffer : public Framebuffer
        {
            friend class VKContext;

        public:
            VKFramebuffer(RenderContext *ctx, VKFramebuffer::Properties *&&pProperties);
            ~VKFramebuffer();

        public:
            VkFramebuffer GetHandle() const { return mFramebuffer; }
            void DestroyImplementation() override;
            void Validate() override {};
            void SetClearColor(const glm::vec4 &color) override;

        private:
            VkFramebuffer mFramebuffer;
        };
    }
}
