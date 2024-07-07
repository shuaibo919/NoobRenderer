#include "Graphics/RHI/IndexBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

IndexBuffer::Builder &IndexBuffer::Builder::SetIndicesData(void *data, uint32_t count)
{
    mProperties->count = count;
    mProperties->data = data;
    return *this;
}
IndexBuffer::Builder &IndexBuffer::Builder::SetUsage(BufferUsage usage)
{
    mProperties->usage = usage;
    return *this;
}
IndexBuffer::Ptr IndexBuffer::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateIndexBuffer(std::move(mProperties));
}
IndexBuffer::IndexBuffer(Properties *&&pProperties)
    : mProperties(pProperties)
{
}
IndexBuffer::~IndexBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}