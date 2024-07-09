#pragma once
#include "Graphics/RHI/UniformBuffer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLShader;

        class GLUniformBuffer : public UniformBuffer
        {
            friend class GLContext;

        public:
            GLUniformBuffer(UniformBuffer::Properties *&&pProperties);
            ~GLUniformBuffer();

        public:
            void Init(uint32_t size, const void *data) override;
            void SetData(uint32_t size, const void *data) override;
            void SetDynamicData(uint32_t size, uint32_t typeSize, const void *data) override;

            void Bind();

        private:
            uint32_t mHandle;
            uint32_t mDynamicSize{0};
            bool mDynamic{false};
        };
    }
}
