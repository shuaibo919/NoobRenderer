#include "Graphics/RHI/IndexBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

IndexBuffer::Builder &IndexBuffer::Builder::SetIndicesData(void *data, uint32_t count, uint16_t indicesType)
{
    mProperties->count = count;
    mProperties->data = data;
    mProperties->indicesType = indicesType;
    return *this;
}

IndexBuffer::Builder &IndexBuffer::Builder::SetUsage(BufferUsage usage)
{
    mProperties->usage = usage;
    return *this;
}

IndexBuffer::Ptr IndexBuffer::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateIndexBuffer(std::move(mProperties));
}

IndexBuffer::IndexBuffer(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

IndexBuffer::~IndexBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}