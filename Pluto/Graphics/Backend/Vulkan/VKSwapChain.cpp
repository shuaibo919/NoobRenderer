/* Vulkan SwapChain */
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKSwapChain::VKSwapChain(RenderContext *ctx, VKSwapChain::Properties *&&pProperties)
    : SwapChain(ctx, std::move(pProperties))
{
}

VKSwapChain::~VKSwapChain()
{
}

void VKSwapChain::OnResize(uint32_t width, uint32_t height)
{
    mProperties->width = width;
    mProperties->height = height;
}

bool VKSwapChain::Init(bool vsync)
{
    return true;
}

Texture::Ptr VKSwapChain::GetCurrentImage()
{
    return nullptr;
}

pluto::SharedPtr<Texture> VKSwapChain::GetImage(uint32_t index)
{
    return nullptr;
};

uint32_t VKSwapChain::GetCurrentImageIndex() const
{
    return 0;
}

void VKSwapChain::SetVSync(bool vsync)
{
}

uint32_t VKSwapChain::GetCurrentBufferIndex() const
{
    return 0;
}

size_t VKSwapChain::GetSwapChainBufferCount() const
{
    return 1;
}
CommandBuffer::Ptr VKSwapChain::GetCurrentCommandBuffer()
{
    return nullptr;
}