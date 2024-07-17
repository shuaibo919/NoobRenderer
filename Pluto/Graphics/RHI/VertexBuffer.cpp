#include "Graphics/RHI/VertexBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

VertexBuffer::Builder &pluto::Graphics::VertexBuffer::Builder::SetVertexData(void *data, uint32_t count, uint32_t totalSize)
{
    mProperties->data = data;
    mProperties->count = count;
    mProperties->vetexSize = totalSize;
    return *this;
}

VertexBuffer::Builder &pluto::Graphics::VertexBuffer::Builder::SetUsage(BufferUsage usage)
{
    mProperties->usage = usage;
    return *this;
}

VertexBuffer::Builder &pluto::Graphics::VertexBuffer::Builder::SetAttribute(
    VertexAttributeType attribute, uint8_t bufferIndex, ElementType attributeType,
    uint32_t byteOffset, uint16_t byteStride, bool normalized)
{
    mProperties->usedAttributes.set(static_cast<uint8_t>(attribute));
    mProperties->attributes[static_cast<uint8_t>(attribute)].offset = byteOffset;
    mProperties->attributes[static_cast<uint8_t>(attribute)].bufferIndex = bufferIndex;
    mProperties->attributes[static_cast<uint8_t>(attribute)].type = attributeType;
    mProperties->attributes[static_cast<uint8_t>(attribute)].stride = byteStride;
    mProperties->attributes[static_cast<uint8_t>(attribute)].normalized = normalized;
    return *this;
}

VertexBuffer::Ptr pluto::Graphics::VertexBuffer::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateVertexBuffer(std::move(mProperties));
}

VertexBuffer::VertexBuffer(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

VertexBuffer::~VertexBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}