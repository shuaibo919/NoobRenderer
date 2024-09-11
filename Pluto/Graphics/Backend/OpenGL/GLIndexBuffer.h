#pragma once
#include "Graphics/RHI/IndexBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLIndexBuffer : public IndexBuffer
        {
            friend class GLContext;

        public:
            GLIndexBuffer(RenderContext *ctx, IndexBuffer::Properties *&&pProperties);
            ~GLIndexBuffer();

            void Bind(CommandBuffer *commandBuffer) const override;
            void Unbind() const override;

        private:
            uint32_t mHandle;
        };
    }
}
