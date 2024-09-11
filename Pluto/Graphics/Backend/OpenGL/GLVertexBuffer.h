#pragma once
#include "Graphics/RHI/VertexBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLVertexBuffer final : public VertexBuffer
        {
            friend class GLContext;

        public:
            GLVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties);
            ~GLVertexBuffer();

        public:
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding = 0) override;
            void Unbind() override;
            const uint32_t GetHandle() const { return mHandle; };

        private:
            uint32_t mHandle;
        };
    }
}
