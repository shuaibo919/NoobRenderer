#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class VertexBuffer
        {
            friend class GraphicsContext;

        public:
            using Ptr = std::shared_ptr<VertexBuffer>;
            struct Properties
            {
                void *data{nullptr};
                uint32_t count{0};
                std::bitset<16> usedAttributes;
                BufferUsage usage{BufferUsage::STATIC};
                std::array<VertexAttribute, VertexAttribute::MAX_VERTEX_COUNT> attributes{};
            };
            struct Builder final : BuildBase<VertexBuffer::Properties, VertexBuffer>
            {
                Builder() {}
                VertexBuffer::Builder &SetVertexData(void *data, uint32_t count);
                VertexBuffer::Builder &SetUsage(BufferUsage usage);
                VertexBuffer::Builder &SetAttribute(VertexAttributeType attribute,
                                                    uint8_t bufferIndex,
                                                    ElementType attributeType,
                                                    uint32_t byteOffset,
                                                    uint8_t byteStride,
                                                    bool normalized = false);

                VertexBuffer::Ptr Create(std::shared_ptr<GraphicsContext> &pContext);
            };
            virtual ~VertexBuffer();
            virtual void Bind(CommandBuffer *commandBuffer, Pipeline *pipeline, uint8_t binding = 0) = 0;
            virtual void Unbind() = 0;

        public:
            uint32_t GetCount() { return mProperties->count; }
            const Properties &GetProperties() const { return *mProperties; }

        protected:
            Properties *mProperties;
            VertexBuffer(Properties *&&pProperties);
            virtual void DelayedBackendInitialize() = 0;
        };
    }
}
