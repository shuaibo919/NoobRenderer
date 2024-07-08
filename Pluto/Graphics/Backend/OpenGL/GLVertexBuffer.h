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
            GLVertexBuffer(VertexBuffer::Properties *&&pProperties);
            ~GLVertexBuffer();

        public:
            void Bind(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Pipeline> pipeline, uint8_t binding = 0) override;
            void Unbind() override;

        private:
            uint32_t mHandle;
        };
    }
}
