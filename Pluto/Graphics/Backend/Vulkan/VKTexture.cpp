/* Vulkan Texture */
#include "Graphics/Backend/Vulkan/VKTexture.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"
using namespace pluto::Graphics;

// VK Texture Helper
namespace vkthelper
{
    VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, uint32_t mipLevels, VkImageViewType viewType, VkImageAspectFlags aspectMask, uint32_t layerCount, uint32_t baseArrayLayer = 0, uint32_t baseMipLevel = 0)
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = viewType;
        viewInfo.format = format;
#if defined(__APPLE__)
        viewInfo.components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY};
#else
        viewInfo.components = {VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A};
#endif
        viewInfo.subresourceRange = {};
        viewInfo.subresourceRange.aspectMask = aspectMask;
        viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
        viewInfo.subresourceRange.levelCount = mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
        viewInfo.subresourceRange.layerCount = layerCount;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            pluto::log<pluto::Info>("Failed to create texture image view!");
        }

        return imageView;
    }

    VkSampler CreateTextureSampler(VkDevice device, VkFilter magFilter = VK_FILTER_LINEAR, VkFilter minFilter = VK_FILTER_LINEAR, float minLod = 0.0f, float maxLod = 1.0f, bool anisotropyEnable = false, float maxAnisotropy = 1.0f, bool compareEnabled = false, VkSamplerAddressMode modeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VkSamplerAddressMode modeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, VkSamplerAddressMode modeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
    {
        VkSampler sampler;
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = magFilter;
        samplerInfo.minFilter = minFilter;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.addressModeU = modeU;
        samplerInfo.addressModeV = modeV;
        samplerInfo.addressModeW = modeW;
        samplerInfo.maxAnisotropy = maxAnisotropy;
        samplerInfo.anisotropyEnable = anisotropyEnable;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE; // compareEnabled;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.compareOp = compareEnabled ? VK_COMPARE_OP_GREATER_OR_EQUAL : VK_COMPARE_OP_NEVER;
        samplerInfo.minLod = minLod;
        samplerInfo.maxLod = maxLod;
        samplerInfo.flags = 0;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
        {
            pluto::log<pluto::Info>("Failed to create texture sampler!");
        }

        return sampler;
    }

    void CreateImageVma(VKRenderDevice *pDevice, const VkImageCreateInfo &imageInfo, VkImage &image, VmaAllocation &allocation)
    {
        VmaAllocationCreateInfo allocInfovma = {};
        allocInfovma.flags = 0;
        allocInfovma.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        allocInfovma.requiredFlags = 0;
        allocInfovma.preferredFlags = 0;
        allocInfovma.memoryTypeBits = 0;
        allocInfovma.pool = nullptr;
        allocInfovma.pUserData = nullptr;

        VmaAllocationInfo alloc_info = {};
        VK_CHECK_RESULT(vmaCreateImage(pDevice->GetAllocator(), &imageInfo, &allocInfovma, &image, &allocation, &alloc_info));
    }

    void CreateImage(VKRenderDevice *pDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageType imageType, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory, uint32_t arrayLayers, VkImageCreateFlags flags, VmaAllocation &allocation, uint32_t samples)

    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = imageType;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = mipLevels;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.queueFamilyIndexCount = 0;
        imageInfo.pQueueFamilyIndices = nullptr;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.samples = samples > 1 ? (VkSampleCountFlagBits)samples : VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.arrayLayers = arrayLayers;
        imageInfo.usage = usage;
        imageInfo.flags = flags;
        CreateImageVma(pDevice, imageInfo, image, allocation);
    }
}

void *VKTexture2D::GetHandle() const
{
    return nullptr;
}

void VKTexture2D::Bind(uint32_t slot) const
{
}

void VKTexture2D::Unbind(uint32_t slot) const
{
}

VKTexture2D::VKTexture2D(RenderContext *ctx, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties)), mTextureImage(VK_NULL_HANDLE),
      mTextureImageView(VK_NULL_HANDLE), mTextureSampler(VK_NULL_HANDLE)
{
    PrepareTexture();
}

VKTexture2D::VKTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
}

VKTexture2D::VKTexture2D(RenderContext *ctx, VkImageView view, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
}

void VKTexture2D::PrepareTexture()
{
    mVKFormat = VKUtilities::GetVKFormat(mProperties->format, mProperties->srgb);
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    if (mProperties->flags & TextureFlags::TextureCreateMips)
    {
        mMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(mProperties->width, mProperties->height)))) + 1;
    }
    else
        mMipLevels = 1;

    auto flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    vkthelper::CreateImage(pBasedDevice, mProperties->width, mProperties->height, mMipLevels, mVKFormat,
                           VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                           mTextureImage, mTextureImageMemory, 1, 0, mAllocation, mProperties->samples);

    mTextureImageView = vkthelper::CreateImageView(pBasedDevice->GetDevice(), mTextureImage, mVKFormat, mMipLevels, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    mTextureSampler = vkthelper::CreateTextureSampler(pBasedDevice->GetDevice(),
                                                      VKUtilities::GetVkTextureFilter(mProperties->minFilter),
                                                      VKUtilities::GetVkTextureFilter(mProperties->magFilter), 0.0f, static_cast<float>(mMipLevels), false, pBasedDevice->GetProperties().limits.maxSamplerAnisotropy,
                                                      VKUtilities::GetVkTextureWrap(mProperties->wrap), VKUtilities::GetVkTextureWrap(mProperties->wrap), VKUtilities::GetVkTextureWrap(mProperties->wrap));

    if (mProperties->flags & TextureFlags::TextureMipViews)
    {
        for (uint32_t i = 0; i < mMipLevels; i++)
        {
            GetMipImageView(i);
        }
    }

    mImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    TransitionImage(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, nullptr);
    mDescriptor.sampler = mTextureSampler;
    mDescriptor.imageView = mTextureImageView;
    mDescriptor.imageLayout = mImageLayout;
}

VkImageView VKTexture2D::GetMipImageView(uint32_t mip)
{
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    if (mMipImageViews.find(mip) == mMipImageViews.end())
    {
        mMipImageViews[mip] = vkthelper::CreateImageView(pBasedDevice->GetDevice(), mTextureImage, mVKFormat, 1, VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT, 1, 0, mip);
    }
    return mMipImageViews.at(mip);
}

void VKTexture2D::TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle)
{
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    if (newLayout != mImageLayout)
        VKUtilities::TransitionImageLayout(mTextureImage, mVKFormat, mImageLayout, newLayout, mMipLevels, 1, vkCmdHandle,
                                           pBasedDevice->GetDevice(), pBasedDevice->GetCommandPool()->GetHandle(), pBasedDevice->GetGraphicsQueue());
    mImageLayout = newLayout;
}

void VKTexture2D::LoadTextureData(const std::string &path)
{
}

VKTexture2D::~VKTexture2D()
{
}

void *VKTexture2DArray::GetHandle() const
{
    return nullptr;
}

void VKTexture2DArray::Bind(uint32_t slot) const
{
}

void VKTexture2DArray::Unbind(uint32_t slot) const
{
}

void VKTexture2DArray::TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle)
{
}

VKTexture2DArray::~VKTexture2DArray()
{
}

VKTexture2DArray::VKTexture2DArray(RenderContext *ctx, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
}

VKTexture2DArray::VKTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
    // mProperties.
}

void *VKTextureCube::GetHandle() const
{
    return nullptr;
}

void VKTextureCube::Bind(uint32_t slot) const
{
}

void VKTextureCube::Unbind(uint32_t slot) const
{
}

void VKTextureCube::TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle)
{
}

VKTextureCube::~VKTextureCube()
{
}

VKTextureCube::VKTextureCube(RenderContext *ctx, Properties *&&pProperties)
    : TextureCube(ctx, std::move(pProperties))
{
}

VKTextureCube::VKTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : VKTextureCube(ctx, std::move(pProperties))
{
    // mProperties.
}

void *VKTexture3D::GetHandle() const
{
    return nullptr;
}

void VKTexture3D::Bind(uint32_t slot) const
{
}

void VKTexture3D::Unbind(uint32_t slot) const
{
}

void VKTexture3D::TransitionImage(VkImageLayout newLayout, VkCommandBuffer vkCmdHandle)
{
}

VKTexture3D::~VKTexture3D()
{
}

VKTexture3D::VKTexture3D(RenderContext *ctx, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}

VKTexture3D::VKTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}
