#include "Graphics/RHI/SwapChain.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

SwapChain::Builder &SwapChain::Builder::SetBase(uint16_t width, uint16_t height)
{
    mProperties->width = width;
    mProperties->height = height;
    return *this;
}
SwapChain::Builder &SwapChain::Builder::SetWindow(Window *window)
{
    mProperties->window = window;
    return *this;
}
SwapChain::Ptr SwapChain::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateSwapChain(mProperties);
}
SwapChain::SwapChain(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}
SwapChain::~SwapChain()
{
    if (mProperties != nullptr)
        delete mProperties;
}