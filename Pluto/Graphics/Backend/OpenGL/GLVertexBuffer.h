#pragma once
#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"

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
            void Bind(CommandBuffer *commandBuffer, Pipeline *pipeline, uint8_t binding = 0) override;
            void Unbind() override;

        protected:
            void DelayedBackendInitialize() override;

        private:
            uint32_t mHandle;
        };
    }
}
