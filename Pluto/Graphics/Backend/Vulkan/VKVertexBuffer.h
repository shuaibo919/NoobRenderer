#pragma once
#include "Graphics/RHI/VertexBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class VKVertexBuffer final : public VertexBuffer
        {
            friend class VKContext;

        public:
            VKVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties);
            ~VKVertexBuffer();

        public:
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding = 0) override;
            void Unbind() override;
        };
    }
}
