#include "Graphics/RHI/DescriptorSet.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;
DescriptorSet::Ptr DescriptorSet::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateDescriptorSet(std::move(mProperties));
}

DescriptorSet::DescriptorSet(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}

DescriptorSet::~DescriptorSet()
{
    if (mProperties != nullptr)
        delete mProperties;
}