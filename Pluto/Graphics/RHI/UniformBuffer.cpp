#include "Graphics/RHI/UniformBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;
UniformBuffer::Ptr pluto::Graphics::UniformBuffer::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateUniformBuffer(std::move(mProperties));
}

UniformBuffer::Builder &pluto::Graphics::UniformBuffer::Builder::SetSize(uint32_t size)
{
    mProperties->size = size;
    return *this;
}

UniformBuffer::UniformBuffer(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

UniformBuffer::~UniformBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}