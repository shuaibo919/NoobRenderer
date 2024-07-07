#include "Graphics/RHI/SwapChain.h"
#include "Graphics/RHI/GraphicsContext.h"

using namespace pluto::Graphics;

SwapChain::Builder &SwapChain::Builder::SetBase(uint16_t width, uint16_t height)
{
    mProperties->width = width;
    mProperties->height = height;
    return *this;
}
SwapChain::Builder &SwapChain::Builder::SetWindow(std::shared_ptr<Window> &Window)
{
    mProperties->window = Window;
    return *this;
}
SwapChain::Ptr SwapChain::Builder::Create(std::shared_ptr<GraphicsContext> &pContext)
{
    return pContext->CreateSwapChain(mProperties);
}
SwapChain::SwapChain(Properties *&&pProperties)
    : mProperties(pProperties)
{
}
SwapChain::~SwapChain()
{
    if (mProperties != nullptr)
        delete mProperties;
}