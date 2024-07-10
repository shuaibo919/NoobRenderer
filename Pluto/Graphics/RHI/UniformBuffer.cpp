#include "Graphics/RHI/UniformBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;
UniformBuffer::Ptr pluto::Graphics::UniformBuffer::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateUniformBuffer(std::move(mProperties));
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