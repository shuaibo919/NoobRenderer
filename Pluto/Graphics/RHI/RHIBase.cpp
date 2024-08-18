/* Vulkan Buffer */
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/RenderContext.h"
using namespace pluto::Graphics;

RHIBase::RHIBase(RenderContext *ctx) : mRenderContext(ctx)
{
    mRenderContext->AttachToRenderContext(this);
}

void RHIBase::DetachFromRenderContext()
{
    mRenderContext->DetachFromRenderContext(this);
}

void RHIBase::DestroyByContext()
{
    this->mDestroyedByContext = true;
    this->DestroyImplementation();
}

void RHIBase::Destroy()
{
    if (this->mDestroyedByContext)
        return;
    this->DetachFromRenderContext();
    this->DestroyImplementation();
}