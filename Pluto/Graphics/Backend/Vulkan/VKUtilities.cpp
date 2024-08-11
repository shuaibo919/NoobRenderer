#include "Graphics/Backend/Vulkan/VKUtilities.h"
#include "Graphics/Window.h"
/* Third */

#if defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#include "Graphics/Backend/Vulkan/VKMacosUtilities.h"
#include <vulkan/vulkan_metal.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#elif defined(_WIN32)
#include <GLFW/glfw3.h>
#elif defined(__linux__)
#endif

#define VKUtils pluto::Graphics::VKUtilities

constexpr std::string GetVKFormatString(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_R8_SRGB:
        return "VK_FORMAT_R8_SRGB";
    case VK_FORMAT_R8G8_SRGB:
        return "VK_FORMAT_R8G8_SRGB";
    case VK_FORMAT_R8G8B8_SRGB:
        return "VK_FORMAT_R8G8B8_SRGB";
    case VK_FORMAT_R8G8B8A8_SRGB:
        return "VK_FORMAT_R8G8B8A8_SRGB";
    case VK_FORMAT_R16G16B16_SFLOAT:
        return "VK_FORMAT_R8G8B8A8_SRGB";
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return "VK_FORMAT_R16G16B16A16_SFLOAT";
    case VK_FORMAT_R32G32B32_SFLOAT:
        return "VK_FORMAT_R32G32B32_SFLOAT";
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return "VK_FORMAT_R32G32B32A32_SFLOAT";
    case VK_FORMAT_R8_UNORM:
        return "VK_FORMAT_R8_UNORM";
    case VK_FORMAT_R8G8_UNORM:
        return "VK_FORMAT_R8G8_UNORM";
    case VK_FORMAT_R8G8B8A8_UNORM:
        return "VK_FORMAT_R8G8B8A8_UNORM";
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        return "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    case VK_FORMAT_D16_UNORM:
        return "VK_FORMAT_D16_UNORM";
    case VK_FORMAT_D32_SFLOAT:
        return "VK_FORMAT_D32_SFLOAT";
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return "VK_FORMAT_D24_UNORM_S8_UINT";
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return "VK_FORMAT_D32_SFLOAT_S8_UINT";
    default:
        break;
    }
    return "Unkonwn";
}

std::string VKUtils::GetErrorString(VkResult result)
{
    switch (result)
    {
    case VK_NOT_READY:
        return "NOT_READY";
    case VK_TIMEOUT:
        return "TIMEOUT";
    case VK_EVENT_SET:
        return "EVENT_SET";
    case VK_EVENT_RESET:
        return "EVENT_RESET";
    case VK_INCOMPLETE:
        return "INCOMPLETE";
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        return "ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        return "ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED:
        return "ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST:
        return "ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED:
        return "ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT:
        return "ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        return "ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT:
        return "ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        return "ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS:
        return "ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        return "ERROR_FORMAT_NOT_SUPPORTED";
    case VK_ERROR_SURFACE_LOST_KHR:
        return "ERROR_SURFACE_LOST_KHR";
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        return "ERROR_NATIVE_WINDOW_IN_USE_KHR";
    case VK_SUBOPTIMAL_KHR:
        return "SUBOPTIMAL_KHR";
    case VK_ERROR_OUT_OF_DATE_KHR:
        return "ERROR_OUT_OF_DATE_KHR";
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        return "ERROR_INCOMPATIBLE_DISPLAY_KHR";
    case VK_ERROR_VALIDATION_FAILED_EXT:
        return "ERROR_VALIDATION_FAILED_EXT";
    case VK_ERROR_INVALID_SHADER_NV:
        return "ERROR_INVALID_SHADER_NV";
    case VK_SUCCESS:
        return "SUCCESS";
    case VK_ERROR_FRAGMENTED_POOL:
        return "ERROR_FRAGMENTED_POOL";
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        return "ERROR_OUT_OF_POOL_MEMORY";
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        return "ERROR_INVALID_EXTERNAL_HANDLE";
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        return "ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
    case VK_ERROR_FRAGMENTATION_EXT:
        return "ERROR_FRAGMENTATION_EXT";
    case VK_ERROR_NOT_PERMITTED_EXT:
        return "ERROR_NOT_PERMITTED_EXT";
    case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
        return "ERROR_INVALID_DEVICE_ADDRESS_EXT";
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        return "ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
    case VK_ERROR_UNKNOWN:
        return "ERROR_UNKNOWN";
    case VK_RESULT_MAX_ENUM:
        return "RESULT_MAX_ENUM";
    default:
        break;
    }
    return "UNKNOWN ERROR";
}

VkSurfaceKHR VKUtils::CreatePlatformSurface(VkInstance inst, pluto::Window *window)
{
    VkSurfaceKHR surface{VK_NULL_HANDLE};
#if defined(_WIN32) && defined(WIN32)
    glfwCreateWindowSurface(inst, static_cast<GLFWwindow *>(window->GetWindowPointer()), nullptr, &surface);
#elif defined(__APPLE__)
    // {vkCreateMacOSSurfaceMVK() is deprecated. Use vkCreateMetalSurfaceEXT() from the VK_EXT_metal_surface extension.}
    VkMetalSurfaceCreateInfoEXT surfaceInfo;
    surfaceInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    surfaceInfo.pNext = NULL;
    surfaceInfo.flags = 0;
    surfaceInfo.pLayer = (CAMetalLayer *)GetCAMetalLayer((void *)glfwGetCocoaWindow(static_cast<GLFWwindow *>(window->GetWindowPointer())));
    vkCreateMetalSurfaceEXT(inst, &surfaceInfo, nullptr, &surface);
#elif defined(__linux__)
    log<Error>("Unix Surface Not Implemented");
#endif

    return surface;
}

VkPresentModeKHR VKUtils::ChoosePresentMode(std::vector<VkPresentModeKHR> availablePresentModes, bool vsync)
{
    // TODO: Implement this
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkFormat VKUtils::GetVKFormat(const RHIFormat format, bool srgb)
{
    if (srgb)
    {
        switch (format)
        {
        case RHIFormat::R8Unorm:
            return VK_FORMAT_R8_SRGB;
        case RHIFormat::R8G8Unorm:
            return VK_FORMAT_R8G8_SRGB;
        case RHIFormat::R8G8B8Unorm:
            return VK_FORMAT_R8G8B8_SRGB;
        case RHIFormat::R8G8B8A8Unorm:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case RHIFormat::R16G16B16Float:
            return VK_FORMAT_R16G16B16_SFLOAT;
        case RHIFormat::R16G16B16A16Float:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case RHIFormat::R32G32B32Float:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case RHIFormat::R32G32B32A32Float:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            log<Info>("Vk-Texture Unsupported image bit-depth!");
            return VK_FORMAT_R8G8B8A8_SRGB;
        }
    }

    switch (format)
    {
    case RHIFormat::R8Unorm:
        return VK_FORMAT_R8_UNORM;
    case RHIFormat::R8G8Unorm:
        return VK_FORMAT_R8G8_UNORM;
    case RHIFormat::R8G8B8Unorm:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case RHIFormat::R8G8B8A8Unorm:
        return VK_FORMAT_R8G8B8A8_UNORM;
    case RHIFormat::R11G11B10Float:
        return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case RHIFormat::R10G10B10A2Unorm:
        return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
    case RHIFormat::R16Float:
        return VK_FORMAT_R16_SFLOAT;
    case RHIFormat::R16G16Float:
        return VK_FORMAT_R16G16_SFLOAT;
    case RHIFormat::R16G16B16Float:
        return VK_FORMAT_R16G16B16_SFLOAT;
    case RHIFormat::R16G16B16A16Float:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    case RHIFormat::R32Float:
        return VK_FORMAT_R32_SFLOAT;
    case RHIFormat::R32G32Float:
        return VK_FORMAT_R32G32_SFLOAT;
    case RHIFormat::R32G32B32Float:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case RHIFormat::R32G32B32A32Float:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    case RHIFormat::Depth16Unorm:
        return VK_FORMAT_D16_UNORM;
    case RHIFormat::Depth32Float:
        return VK_FORMAT_D32_SFLOAT;
    case RHIFormat::Depth24UnormStencil8UInt:
        return VK_FORMAT_D24_UNORM_S8_UINT;
    case RHIFormat::Depth32FloatStencil8UInt:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
    default:
        log<Info>("Vk-Texture Unsupported image bit-depth!");
        break;
    }
    return VK_FORMAT_R8G8B8A8_UNORM;
}

VkSamplerAddressMode VKUtils::GetVkTextureWrap(const TextureWrap wrap)
{
    switch (wrap)
    {
    case TextureWrap::Clmap:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureWrap::ClampToBorder:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case TextureWrap::ClampToedge:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case TextureWrap::Repeat:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case TextureWrap::MirroredRepeat:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    default:
        log<Info>("Vk-Texture Unsupported wrap type!");
        break;
    }
    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
}

VkFilter VKUtils::GetVkTextureFilter(const TextureFilter filter)
{
    switch (filter)
    {
    case TextureFilter::Nearest:
        return VK_FILTER_NEAREST;
    case TextureFilter::Linear:
        return VK_FILTER_LINEAR;
    case TextureFilter::None:
        return VK_FILTER_LINEAR;
    default:
        log<Info>("Vk-Texture] Unsupported TextureFilter type!");
        break;
    }
    return VK_FILTER_LINEAR;
}

bool VKUtils::HasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkPipelineStageFlags VKUtils::AccessFlagsToPipelineStage(VkAccessFlags accessFlags, const VkPipelineStageFlags stageFlags)
{
    VkPipelineStageFlags stages = 0;

    while (accessFlags != 0)
    {
        VkAccessFlagBits AccessFlag = static_cast<VkAccessFlagBits>(accessFlags & (~(accessFlags - 1)));
        accessFlags &= ~AccessFlag;

        switch (AccessFlag)
        {
        case VK_ACCESS_INDIRECT_COMMAND_READ_BIT:
            stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
            break;

        case VK_ACCESS_INDEX_READ_BIT:
            stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            break;

        case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:
            stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
            break;

        case VK_ACCESS_UNIFORM_READ_BIT:
            stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            break;

        case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT:
            stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            break;

        case VK_ACCESS_SHADER_READ_BIT:
            stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            break;

        case VK_ACCESS_SHADER_WRITE_BIT:
            stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
            break;

        case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT:
            stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;

        case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:
            stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            break;

        case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:
            stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            break;

        case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:
            stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            break;

        case VK_ACCESS_TRANSFER_READ_BIT:
            stages |= VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_ACCESS_TRANSFER_WRITE_BIT:
            stages |= VK_PIPELINE_STAGE_TRANSFER_BIT;
            break;

        case VK_ACCESS_HOST_READ_BIT:
            stages |= VK_PIPELINE_STAGE_HOST_BIT;
            break;

        case VK_ACCESS_HOST_WRITE_BIT:
            stages |= VK_PIPELINE_STAGE_HOST_BIT;
            break;

        case VK_ACCESS_MEMORY_READ_BIT:
            break;

        case VK_ACCESS_MEMORY_WRITE_BIT:
            break;

        default:
            log<Error>("Unknown access flag");
            break;
        }
    }
    return stages;
}

VkPipelineStageFlags VKUtils::LayoutToAccessMask(const VkImageLayout layout, const bool isDestination)
{
    VkPipelineStageFlags accessMask = 0;

    switch (layout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        if (isDestination)
        {
            log<Error>("The new layout used in a transition must not be VK_IMAGE_LAYOUT_UNDEFINED.");
        }
        break;

    case VK_IMAGE_LAYOUT_GENERAL:
        accessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        accessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        accessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        accessMask = VK_ACCESS_TRANSFER_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        accessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        if (!isDestination)
        {
            accessMask = VK_ACCESS_HOST_WRITE_BIT;
        }
        else
        {
            log<Error>("The new layout used in a transition must not be VK_IMAGE_LAYOUT_PREINITIALIZED.");
        }
        break;

    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
        accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
        accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        break;

    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        accessMask = VK_ACCESS_2_NONE;
        break;

    default:
        log<Error>("Unexpected image layout");
        break;
    }

    return accessMask;
}

bool VKUtils::IsDepthFormat(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_D16_UNORM:
    case VK_FORMAT_D32_SFLOAT:
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
    case VK_FORMAT_D16_UNORM_S8_UINT:
        return true;
    default:
        break;
    }
    return false;
}

bool VKUtils::IsStencilFormat(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return true;
    default:
        break;
    }
    return false;
}

void VKUtils::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
                                    uint32_t mipLevels, uint32_t layerCount, VkCommandBuffer commandBuffer,
                                    VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)
{

    bool singleTimeCommand = false;

    if (!commandBuffer)
    {
        commandBuffer = BeginSingleTimeCommands(device, commandPool);
        singleTimeCommand = true;
    }

    VkImageSubresourceRange subresourceRange = {};
    subresourceRange.aspectMask = IsDepthFormat(format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;

    if (IsStencilFormat(format))
        subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = layerCount;
    subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

    // Create an image barrier object
    VkImageMemoryBarrier imageMemoryBarrier = {};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;
    imageMemoryBarrier.srcAccessMask = LayoutToAccessMask(oldImageLayout, false);
    imageMemoryBarrier.dstAccessMask = LayoutToAccessMask(newImageLayout, true);
    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    VkPipelineStageFlags sourceStage = 0;
    {
        if (imageMemoryBarrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        {
            sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        else if (imageMemoryBarrier.srcAccessMask != 0)
        {
            sourceStage = AccessFlagsToPipelineStage(imageMemoryBarrier.srcAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        }
        else
        {
            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        }
    }

    VkPipelineStageFlags destinationStage = 0;
    {
        if (imageMemoryBarrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
        {
            destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        }
        else if (imageMemoryBarrier.dstAccessMask != 0)
        {
            destinationStage = AccessFlagsToPipelineStage(imageMemoryBarrier.dstAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
        }
        else
        {
            destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        }
    }

    // Put barrier inside setup command buffer
    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage,
        destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier);

    if (singleTimeCommand)
        EndSingleTimeCommands(device, commandPool, graphicsQueue, commandBuffer);
}

VkCommandBuffer VKUtils::BeginSingleTimeCommands(VkDevice device, VkCommandPool commandPool)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo));

    return commandBuffer;
}

void VKUtils::EndSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer)
{
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.pSignalSemaphores = nullptr;
    submitInfo.pNext = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.waitSemaphoreCount = 0;

    VK_CHECK_RESULT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));
    VK_CHECK_RESULT(vkQueueWaitIdle(graphicsQueue));

    vkFreeCommandBuffers(device,
                         commandPool, 1, &commandBuffer);
}

void VKUtils::CopyBufferToImage(VkBuffer buffer, VkImage image, uint16_t width, uint16_t height, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue)
{
    VkCommandBuffer commandBuffer = BeginSingleTimeCommands(device, commandPool);

    VkBufferImageCopy region;
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {
        width,
        height,
        1};

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    VKUtilities::EndSingleTimeCommands(device, commandPool, graphicsQueue, commandBuffer);
}

pluto::Graphics::RHIFormat VKUtils::GetRHIFormat(VkFormat format)
{
    switch (format)
    {
    case VK_FORMAT_R8_SRGB:
        return RHIFormat::R8Unorm;
    case VK_FORMAT_R8G8_SRGB:
        return RHIFormat::R8G8Unorm;
    case VK_FORMAT_R8G8B8_SRGB:
        return RHIFormat::R8G8B8Unorm;
    case VK_FORMAT_R8G8B8A8_SRGB:
        return RHIFormat::R8G8B8A8Unorm;
    case VK_FORMAT_R16G16B16_SFLOAT:
        return RHIFormat::R16G16B16Float;
    case VK_FORMAT_R16G16B16A16_SFLOAT:
        return RHIFormat::R16G16B16A16Float;
    case VK_FORMAT_R32G32B32_SFLOAT:
        return RHIFormat::R32G32B32Float;
    case VK_FORMAT_R32G32B32A32_SFLOAT:
        return RHIFormat::R32G32B32A32Float;
    case VK_FORMAT_R8_UNORM:
        return RHIFormat::R8Unorm;
    case VK_FORMAT_R8G8_UNORM:
        return RHIFormat::R8G8Unorm;
    case VK_FORMAT_R8G8B8A8_UNORM:
        return RHIFormat::R8G8B8A8Unorm;
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        return RHIFormat::R11G11B10Float;
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        return RHIFormat::R10G10B10A2Unorm;
    case VK_FORMAT_D16_UNORM:
        return RHIFormat::Depth16Unorm;
    case VK_FORMAT_D32_SFLOAT:
        return RHIFormat::Depth32Float;
    case VK_FORMAT_D24_UNORM_S8_UINT:
        return RHIFormat::Depth24UnormStencil8UInt;
    case VK_FORMAT_D32_SFLOAT_S8_UINT:
        return RHIFormat::Depth32FloatStencil8UInt;
    default:
        PLog<PError>("[%s] Unsupported texture type: %s|%d  ", PLineInfo, GetVKFormatString(format).c_str(), format);
        break;
    }
    return RHIFormat::R8G8B8A8Unorm;
}

VkShaderStageFlagBits VKUtils::GetShaderType(const ShaderType &shaderName)
{
    switch (shaderName)
    {
    case ShaderType::Vertex:
        return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderType::Geometry:
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    case ShaderType::Fragment:
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderType::Compute:
        return VK_SHADER_STAGE_COMPUTE_BIT;
    default:
        PLog<PError>("[%s] Unsupported shader type: ShaderType::Unkonwn ", PLineInfo);
        break;
    }
    return VK_SHADER_STAGE_VERTEX_BIT;
}

void VKUtils::SetDebugUtilsObjectNameInfo(VkInstance inst, const VkDevice device, const VkObjectType objectType, const char *name, const void *handle)
{
    VkDebugUtilsObjectNameInfoEXT nameInfo;
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = objectType;
    nameInfo.pObjectName = name;
    nameInfo.objectHandle = (uint64_t)handle;
    nameInfo.pNext = VK_NULL_HANDLE;

    auto pSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(inst, "vkSetDebugUtilsObjectNameEXT"));
    if (pSetDebugUtilsObjectNameEXT != nullptr)
        VK_CHECK_RESULT(pSetDebugUtilsObjectNameEXT(device, &nameInfo));
}

VkDescriptorType VKUtils::GetDescriptorType(DescriptorType type)
{
    switch (type)
    {
    case DescriptorType::UniformBuffer:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case DescriptorType::UniformBufferDynamic:
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    case DescriptorType::ImageSampler:
        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    case DescriptorType::ImageStorage:
        return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    default:
        break;
    }
    PLog<PError>("[%s] Unsupported Descriptor Type ", PLineInfo);
    return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}
