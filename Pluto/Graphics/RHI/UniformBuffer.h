#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class UniformBuffer : public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = std::shared_ptr<UniformBuffer>;
            struct Properties
            {
                uint8_t *data;
                uint32_t size;
            };
            struct Builder final : BuildBase<UniformBuffer::Properties, UniformBuffer>
            {
                Builder() {}
                UniformBuffer::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~UniformBuffer();

        public:
            virtual void Init(uint32_t size, const void *data) = 0;
            virtual void SetData(uint32_t size, const void *data) = 0;
            virtual void SetDynamicData(uint32_t size, uint32_t typeSize, const void *data) = 0;

        public:
            const Properties &GetProperties() const { return *mProperties; }

        protected:
            Properties *mProperties;
            UniformBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
