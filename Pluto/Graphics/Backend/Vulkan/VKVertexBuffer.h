#pragma once
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKBuffer;
        class VKVertexBuffer final : public VertexBuffer
        {
            friend class VKContext;

        public:
            VKVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties);
            ~VKVertexBuffer();

        public:
            void DestroyImplementation() override;
            void Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding = 0) override;
            void Unbind() override;

        public:
            VKBuffer *&GetBuffer() { return mBuffer; }

        private:
            VKBuffer *mBuffer{nullptr};
        };
    }
}
