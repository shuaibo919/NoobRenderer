#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class IndexBuffer : public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = std::shared_ptr<IndexBuffer>;
            struct Properties
            {
                void *data{nullptr};
                uint16_t indicesType;
                uint32_t count{0};
                BufferUsage usage{BufferUsage::Static};
            };
            struct Builder final : BuildBase<IndexBuffer::Properties, IndexBuffer>
            {
                Builder() {}
                IndexBuffer::Builder &SetIndicesData(void *data, uint32_t count, uint16_t indicesType = sizeof(uint16_t));
                IndexBuffer::Builder &SetUsage(BufferUsage usage);
                IndexBuffer::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~IndexBuffer();

            const Properties &GetProperties() const { return *mProperties; }

            virtual void Bind(CommandBuffer *commandBuffer = nullptr) const = 0;
            virtual void Unbind() const = 0;

        protected:
            Properties *mProperties;
            IndexBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
