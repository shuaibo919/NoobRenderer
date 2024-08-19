#pragma once
#include "Graphics/RHI/IndexBuffer.h"
#include "Graphics/Backend/Vulkan/Vk.h"
namespace pluto
{
    namespace Graphics
    {
        class VKIndexBuffer : public IndexBuffer, public VKObjectManageByContext
        {
            friend class VKContext;

        public:
            VKIndexBuffer(RenderContext *ctx, IndexBuffer::Properties *&&pProperties);
            ~VKIndexBuffer();

            void DestroyImplementation() override;
            void Bind(CommandBuffer *commandBuffer) const override;
            void Unbind() const override;
        };
    }
}
