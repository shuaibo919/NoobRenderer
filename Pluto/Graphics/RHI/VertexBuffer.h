#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class VertexBuffer : public std::enable_shared_from_this<VertexBuffer>, public RHIBase
        {
            friend class GraphicsContext;

        public:
            using Ptr = SharedPtr<VertexBuffer>;
            struct Properties
            {
                void *data{nullptr};
                uint32_t count{0};
                uint16_t vetexSize{0};
                std::bitset<16> usedAttributes;
                BufferUsage usage{BufferUsage::Static};
                std::array<VertexAttribute, VertexAttribute::MAX_VERTEX_COUNT> attributes{};
            };
            struct Builder final : BuildBase<VertexBuffer::Properties, VertexBuffer>
            {
                Builder() {}
                VertexBuffer::Builder &SetVertexData(void *data, uint32_t count, uint32_t totalSize);
                VertexBuffer::Builder &SetUsage(BufferUsage usage);
                VertexBuffer::Builder &SetAttribute(VertexAttributeType attribute,
                                                    uint8_t bufferIndex,
                                                    ElementType attributeType,
                                                    uint32_t byteOffset,
                                                    uint16_t byteStride,
                                                    bool normalized = false);

                VertexBuffer::Ptr Create(const SharedPtr<GraphicsContext> &pContext);
            };
            virtual ~VertexBuffer();
            VertexBuffer::Ptr Get()
            {
                return shared_from_this();
            }
            virtual void Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding = 0) = 0;
            virtual void Unbind() = 0;

        public:
            uint32_t GetCount() { return mProperties->count; }
            const Properties &GetProperties() const { return *mProperties; }

        protected:
            Properties *mProperties;
            VertexBuffer(RenderContext *ctx, Properties *&&pProperties);
        };
    }
}
