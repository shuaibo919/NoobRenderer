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
    this->FindImageFormatAndColourSpace();

    if (!mSurface)
    {
        log<Error>("Failed to create window surface!");
    }

    if (mSurface == VK_NULL_HANDLE)
        mSurface = VKUtilities::CreatePlatformSurface();

    VkBool32 queueIndexSupported;
    vkGetPhysicalDeviceSurfaceSupportKHR(mBasedDevice->GetGPU(), mBasedDevice->GetGraphicsQueueFamilyIndex(), mSurface, &queueIndexSupported);
    // Swap chain
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mBasedDevice->GetGPU(), mSurface, &surfaceCapabilities);

    uint32_t numPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(mBasedDevice->GetGPU(), mSurface, &numPresentModes, VK_NULL_HANDLE);

    std::vector<VkPresentModeKHR> pPresentModes(numPresentModes);
    vkGetPhysicalDeviceSurfacePresentModesKHR(mBasedDevice->GetGPU(), mSurface, &numPresentModes, pPresentModes.data());

    VkExtent2D swapChainExtent;

    swapChainExtent.width = static_cast<uint32_t>(mProperties->width);
    swapChainExtent.height = static_cast<uint32_t>(mProperties->height);

    VkPresentModeKHR swapChainPresentMode = VKUtilities::ChoosePresentMode(pPresentModes, vsync);

    mSwapChainBufferCount = surfaceCapabilities.maxImageCount;
    mSwapChainBufferCount = std::clamp(mSwapChainBufferCount, static_cast<uint32_t>(0), static_cast<uint32_t>(3));

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    else
        preTransform = surfaceCapabilities.currentTransform;

    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (auto &compositeAlphaFlag : compositeAlphaFlags)
    {
        if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag)
        {
            compositeAlpha = compositeAlphaFlag;
            break;
        };
    }

    VkSwapchainCreateInfoKHR swapChainInfo{};
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = mSurface;
    swapChainInfo.minImageCount = mSwapChainBufferCount;
    swapChainInfo.imageFormat = mColourFormat;
    swapChainInfo.imageExtent.width = swapChainExtent.width;
    swapChainInfo.imageExtent.height = swapChainExtent.height;
    swapChainInfo.preTransform = preTransform;
    swapChainInfo.compositeAlpha = compositeAlpha;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.presentMode = swapChainPresentMode;
    swapChainInfo.oldSwapchain = mOldSwapChain;
    swapChainInfo.imageColorSpace = mColourSpace;
    swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainInfo.queueFamilyIndexCount = 0;
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainInfo.pQueueFamilyIndices = VK_NULL_HANDLE;
    swapChainInfo.clipped = VK_TRUE;

    if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
    {
        swapChainInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    }

    if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    {
        swapChainInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    VK_CHECK_RESULT(vkCreateSwapchainKHR(mBasedDevice->GetDevice(), &swapChainInfo, VK_NULL_HANDLE, &mSwapChain));

    if (mOldSwapChain != VK_NULL_HANDLE)
    {
        for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
        {
            // todo: if cmdbuffer is not finished, wait for it to finish

            mFrames[i].MainCommandBuffer->Reset();

            mSwapChainBuffers[i].reset();
            mFrames[i].ImageAcquireSemaphore = VK_NULL_HANDLE;
        }

        mSwapChainBuffers.clear();

        vkDestroySwapchainKHR(mBasedDevice->GetDevice(), mOldSwapChain, VK_NULL_HANDLE);
        mOldSwapChain = VK_NULL_HANDLE;
    }

    uint32_t swapChainImageCount;
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(mBasedDevice->GetDevice(), mSwapChain, &swapChainImageCount, VK_NULL_HANDLE));

    std::vector<VkImage> swapChainImages(swapChainImageCount);
    VK_CHECK_RESULT(vkGetSwapchainImagesKHR(mBasedDevice->GetDevice(), mSwapChain, &swapChainImageCount, swapChainImages.data()));

    for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
    {
        if (i >= swapChainImageCount)
            break;

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.format = mColourFormat;
#ifdef VK_USE_PLATFORM_MACOS_MVK
        viewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
#else
        viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
#endif
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.flags = 0;
        viewInfo.image = swapChainImages[i];

        VkImageView imageView;
        VK_CHECK_RESULT(vkCreateImageView(mBasedDevice->GetDevice(), &viewInfo, VK_NULL_HANDLE, &imageView));

        auto properties = new VKTexture2D::Properties();
        auto swapChainBuffer = Vulkan::CreateTexture(static_cast<uint16_t>(Texture::Type::Texture2D), mRenderContext, imageView, std::move(properties));
        std::static_pointer_cast<VKTexture2D>(swapChainBuffer)->TransitionImage(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_NULL_HANDLE);
        mSwapChainBuffers.push_back(swapChainBuffer);
    }

    PrepareFrameData();

    return true;
}

void VKSwapChain::PrepareFrameData()
{
}

void VKSwapChain::FindImageFormatAndColourSpace()
{
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