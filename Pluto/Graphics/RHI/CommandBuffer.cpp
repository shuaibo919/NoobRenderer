#include "Graphics/RHI/CommandBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;
CommandBuffer::Ptr pluto::Graphics::CommandBuffer::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateCommandBuffer(std::move(mProperties));
}
CommandBuffer::CommandBuffer(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}
CommandBuffer::~CommandBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}