/* Vulkan SwapChain */
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKSwapChain::VKSwapChain(RenderContext *ctx, VKSwapChain::Properties *&&pProperties)
    : SwapChain(ctx, std::move(pProperties)), mSwapChain(VK_NULL_HANDLE), mOldSwapChain(VK_NULL_HANDLE), mSurface(VK_NULL_HANDLE),
      mCurrentBuffer(0), mAcquireImageIndex(std::numeric_limits<uint32_t>::max())
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

bool VKSwapChain::Init(bool vsync, const SharedPtr<RenderDevice> &pDevice)
{
    if (pDevice == nullptr)
    {
        return false;
        log<Error>("Must pass a valid device pointer to create a swapchain in VK-backend!");
    }
    mBasedDevice = std::static_pointer_cast<VKRenderDevice>(pDevice);
    FindImageFormatAndColourSpace();

    if (!mSurface)
    {
        log<Error>("Failed to create window surface!");
    }

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