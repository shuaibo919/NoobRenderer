#include "Graphics/RHI/CommandBuffer.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;
CommandBuffer::Ptr pluto::Graphics::CommandBuffer::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateCommandBuffer(std::move(mProperties));
}
CommandBuffer::CommandBuffer(Properties *&&pProperties)
    : mProperties(pProperties)
{
}
CommandBuffer::~CommandBuffer()
{
    if (mProperties != nullptr)
        delete mProperties;
}