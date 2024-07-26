#pragma once
#include "Graphics/RHI/IndexBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class VKIndexBuffer : public IndexBuffer
        {
            friend class VKContext;

        public:
            VKIndexBuffer(RenderContext *ctx, IndexBuffer::Properties *&&pProperties);
            ~VKIndexBuffer();

            void Bind(CommandBuffer *commandBuffer) const override;
            void Unbind() const override;
        };
    }
}
