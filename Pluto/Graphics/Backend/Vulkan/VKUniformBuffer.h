#pragma once
#include "Graphics/RHI/UniformBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class VKShader;
        class VKBuffer;
        class VKUniformBuffer : public UniformBuffer
        {
            friend class VKContext;

        public:
            VKUniformBuffer(RenderContext *ctx, UniformBuffer::Properties *&&pProperties);
            ~VKUniformBuffer();

        public:
            void DestroyImplementation() override;
            void ReInit(uint32_t size, void *data) override;
            void SetData(uint32_t size, void *data) override;
            void SetDynamicData(uint32_t size, uint32_t typeSize, void *data) override;

        public:
            VKBuffer *&GetBuffer() { return mBuffer; }

        private:
            VKBuffer *mBuffer{nullptr};
        };
    }
}
