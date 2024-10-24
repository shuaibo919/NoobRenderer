#include "Graphics/RHI/RenderCommand.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

RenderCommand::Builder &RenderCommand::Builder::SetUsageType(CommandBufferUsageType type)
{
    mProperties->type = type;
    return *this;
}

RenderCommand::Ptr pluto::Graphics::RenderCommand::Builder::Create(const SharedPtr<GraphicsContext> &pContext)
{
    return pContext->CreateRenderCommand(std::move(mProperties));
}

RenderCommand::RenderCommand(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

RenderCommand::~RenderCommand()
{
    if (mProperties != nullptr)
        delete mProperties;
}