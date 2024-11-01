/* Vulkan SwapChain */
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderCommand.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"
/* Window */
#include "Graphics/Window.h"
#include "VKSwapChain.h"

using namespace pluto::Graphics;

VKSwapChain::VKSwapChain(RenderContext *ctx, VKSwapChain::Properties *&&pProperties)
    : SwapChain(ctx, std::move(pProperties)), mSwapChain(VK_NULL_HANDLE), mOldSwapChain(VK_NULL_HANDLE),
      mSurface(VK_NULL_HANDLE),
      mCurrentBuffer(0),
      mAcquireImageIndex((std::numeric_limits<uint32_t>::max)())
{
}

VKSwapChain::~VKSwapChain()
{
    auto pRenderCtx = static_cast<VKRenderContext *>(mRenderContext);
    vkDeviceWaitIdle(mBasedDevice->GetDevice());

    for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
    {
        mFrames[i].CommandPool.reset();
        mFrames[i].ImageAcquireSemaphore.reset();

        mSwapChainBuffers[i].reset();
    }
    vkDestroySwapchainKHR(mBasedDevice->GetDevice(), mSwapChain, VK_NULL_HANDLE);

    if (mSurface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(pRenderCtx->GetVKInstance(), mSurface, nullptr);
    }
}

void VKSwapChain::BeginFrame()
{
    // auto &commandBuffer = mFrames[mCurrentBuffer].CommandBuffer;
    // if (commandBuffer->GetState() == CommandBufferState::Submitted)
    // {
    //     if (!commandBuffer->Wait())
    //     {
    //         log<Error>("Failed to wait for command buffer!");
    //         return;
    //     }
    // }
    // commandBuffer->Reset();
    AcquireNextImage();
}

void VKSwapChain::EndFrame()
{

    std::vector<VkSemaphore> semaphores;
    if (mFrames[mCurrentBuffer].CachedCommandBuffer != nullptr)
    {
        semaphores.push_back(mFrames[mCurrentBuffer].CachedCommandBuffer->GetSemaphore());
        mFrames[mCurrentBuffer].CachedCommandBuffer = nullptr;
    }

    auto cnt = static_cast<uint32_t>(std::count_if(mFrames, mFrames + mSwapChainBufferCount, [](const FrameData &frame)
                                 { return frame.CachedCommandBuffer == nullptr; }));

    if (cnt == mSwapChainBufferCount)
        mCurrentRenderCommand = nullptr;

    this->Present(semaphores);
    mRenderContext->WaitIdle();
    mCurrentBuffer = (mCurrentBuffer + 1) % mSwapChainBufferCount;
}

void VKSwapChain::OnResize(uint32_t width, uint32_t height)
{
    if (mProperties->width == width && mProperties->height == height)
        return;

    vkDeviceWaitIdle(mBasedDevice->GetDevice());

    mProperties->width = width;
    mProperties->height = height;
    mOldSwapChain = mSwapChain;
    mSwapChain = VK_NULL_HANDLE;
    this->Init(mVSyncEnabled);

    vkDeviceWaitIdle(mBasedDevice->GetDevice());
}

bool VKSwapChain::Init(bool vsync)
{
    auto pRenderCtx = static_cast<VKRenderContext *>(mRenderContext);
    mBasedDevice = pRenderCtx->GetBasedDevice();

    if (mSurface == VK_NULL_HANDLE)
        mSurface = VKUtilities::CreatePlatformSurface(pRenderCtx->GetVKInstance(), mProperties->window);

    if (!mSurface)
    {
        log<Error>("Failed to create window surface!");
    }
    this->FindImageFormatAndColourSpace();

    VkBool32 queueIndexSupported;
    vkGetPhysicalDeviceSurfaceSupportKHR(mBasedDevice->GetGPU(), mBasedDevice->GetGraphicsQueueFamilyIndex(), mSurface, &queueIndexSupported);

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
    if (mSwapChainBufferCount > SwapChain::MaxFlightFrames)
        mSwapChainBufferCount = 3;
    else if (mSwapChainBufferCount == 0)
        mSwapChainBufferCount = 3;

    log<Info>("Swap Chain Buffer Count: %d", mSwapChainBufferCount);

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
        log<Info>("Reset old swapchain");
        for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
        {
            mSwapChainBuffers[i].reset();
            mFrames[i].ImageAcquireSemaphore = nullptr;
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
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.format = mColourFormat;
        viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
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
        properties->width = mProperties->width;
        properties->height = mProperties->height;
        auto swapChainBuffer = utilities::protected_make_shared<VKTexture2D>(mRenderContext, swapChainImages[i], imageView, mColourFormat, std::move(properties));
        swapChainBuffer->TransitionImage(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_NULL_HANDLE);
        mSwapChainBuffers.push_back(swapChainBuffer);
    }

    PrepareFrameData();

    return true;
}

void VKSwapChain::Submit(SharedPtr<RenderCommand> command)
{
    auto pRenderCommandSubmit = std::static_pointer_cast<VKRenderCommand>(command);
    auto vkSyncSemaphore = (mFrames[mCurrentBuffer].CachedCommandBuffer == nullptr)
                               ? mFrames[mCurrentBuffer].ImageAcquireSemaphore->GetHandle()
                               : mCurrentRenderCommand->GetCommandBuffer(mCurrentBuffer)->GetSemaphore();
    pRenderCommandSubmit->Execute(mCurrentBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, mFrames[mCurrentBuffer].ImageAcquireSemaphore->GetHandle(), false);
    mCurrentRenderCommand = pRenderCommandSubmit;
    mFrames[mCurrentBuffer].CachedCommandBuffer = mCurrentRenderCommand->GetCommandBuffer(mCurrentBuffer);
}

void VKSwapChain::PrepareFrameData()
{
    for (uint32_t i = 0; i < mSwapChainBufferCount; i++)
    {
        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;

        mFrames[i].ImageAcquireSemaphore = std::make_shared<VKSemaphore>(mBasedDevice->GetDevice(), false);
        if (mFrames[i].CommandPool == nullptr)
        {
            mFrames[i].CommandPool = std::make_shared<VKCommandPool>(mBasedDevice->GetDevice(), mBasedDevice->GetGraphicsQueueFamilyIndex(), VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        }
    }
}

void VKSwapChain::AcquireNextImage()
{
    static int FailedCount = 0;
    if (mSwapChainBufferCount == 1 && mAcquireImageIndex != (std::numeric_limits<uint32_t>::max)())
    {
        return;
    }

    auto result = vkAcquireNextImageKHR(mBasedDevice->GetDevice(), mSwapChain, UINT64_MAX,
                                        mFrames[mCurrentBuffer].ImageAcquireSemaphore->GetHandle(), VK_NULL_HANDLE, &mAcquireImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        log<Info>("Acquire Image result : %s", result == VK_ERROR_OUT_OF_DATE_KHR ? "Out of Date" : "SubOptimal");

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            this->OnResize(mProperties->width, mProperties->height);
        }
    }
    else if (result != VK_SUCCESS)
    {
        FailedCount++;
        log<Critical>("Failed to acquire swap chain image! - %s", VKUtilities::GetErrorString(result).c_str());

        if (FailedCount > 10)
        {
            log<Critical>(" Failed to acquire swap chain image %d times!", FailedCount);
            NRE_ASSERT(false, "Failed to acquire swap chain image to many times!");
        }
        return;
    }

    FailedCount = 0;
}

void VKSwapChain::FindImageFormatAndColourSpace()
{
    uint32_t formatCount;
    VkPhysicalDevice physicalDevice = mBasedDevice->GetGPU();
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatCount, nullptr));

    std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, mSurface, &formatCount, surfaceFormats.data()));

    if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
    {
        mColourFormat = VK_FORMAT_B8G8R8A8_UNORM;
        mColourSpace = surfaceFormats[0].colorSpace;
    }
    else
    {
        bool found = false;
        for (auto &&surfaceFormat : surfaceFormats)
        {
            if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
            {
                mColourFormat = surfaceFormat.format;
                mColourSpace = surfaceFormat.colorSpace;
                found = true;
                break;
            }
        }
        if (!found)
        {
            PLog<PInfo>("Not found default format, use the first");
            mColourFormat = surfaceFormats[0].format;
            mColourSpace = surfaceFormats[0].colorSpace;
        }
    }
}

FrameData &VKSwapChain::GetCurrentFrameData()
{
    return mFrames[mCurrentBuffer];
}

VkCommandPool VKSwapChain::GetFrameCommandPool(uint32_t index) const
{
    return mFrames[index].CommandPool->GetHandle();
}

Texture::Ptr VKSwapChain::GetCurrentImage()
{
    return mSwapChainBuffers[mAcquireImageIndex];
}

pluto::SharedPtr<Texture> VKSwapChain::GetImage(uint32_t index)
{
    return mSwapChainBuffers[index];
};

uint32_t VKSwapChain::GetCurrentImageIndex() const
{
    // if (mAcquireImageIndex == (std::numeric_limits<uint32_t>::max)()) [[unlikely]]
    //     return 0;
    return mAcquireImageIndex;
}

void VKSwapChain::SetVSync(bool vsync)
{
    mVSyncEnabled = vsync;
}

uint32_t VKSwapChain::GetCurrentBufferIndex() const
{
    return mCurrentBuffer;
}

size_t VKSwapChain::GetSwapChainBufferCount() const
{
    return mSwapChainBufferCount;
}

RenderCommand::Ptr VKSwapChain::GetCurrentRenderCommand()
{
    return mCurrentRenderCommand;
}

void VKSwapChain::Present(const std::vector<VkSemaphore> &semaphore)
{
    VkPresentInfoKHR present;
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = VK_NULL_HANDLE;
    present.swapchainCount = 1;
    present.pSwapchains = &mSwapChain;
    present.pImageIndices = &mAcquireImageIndex;
    present.waitSemaphoreCount = static_cast<uint32_t>(semaphore.size());
    present.pWaitSemaphores = semaphore.data();
    present.pResults = VK_NULL_HANDLE;

    auto error = vkQueuePresentKHR(mBasedDevice->GetPresentQueue(), &present);

    if (error == VK_ERROR_OUT_OF_DATE_KHR)
    {
        log<Error>("SwapChain out of date");
    }
    else if (error == VK_SUBOPTIMAL_KHR)
    {
        log<Error>("SwapChain suboptimal");
    }
    else
    {
        VK_CHECK_RESULT(error);
    }

    vkQueueWaitIdle(mBasedDevice->GetPresentQueue());
}
