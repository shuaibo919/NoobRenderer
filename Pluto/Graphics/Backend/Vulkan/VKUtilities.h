#pragma once
#include "Graphics/RHI/Declarations.h"
#include "Graphics/RHI/Utilities.h"
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    class Window;
    namespace Graphics
    {
        namespace VKUtilities
        {
            std::string GetErrorString(VkResult result);
            VkSurfaceKHR CreatePlatformSurface(VkInstance inst, Window *window);
            VkPresentModeKHR ChoosePresentMode(std::vector<VkPresentModeKHR> availablePresentModes, bool vsync);
            bool HasStencilComponent(VkFormat format);
            VkFormat GetVKFormat(const RHIFormat format, bool srgb);
            RHIFormat GetRHIFormat(VkFormat format);
            VkSamplerAddressMode GetVkTextureWrap(const TextureWrap wrap);
            VkFilter GetVkTextureFilter(const TextureFilter filter);
            VkCommandBuffer BeginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
            void EndSingleTimeCommands(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkCommandBuffer commandBuffer);
            void CopyBufferToImage(VkBuffer buffer, VkImage image, uint16_t width, uint16_t height,
                                   VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
            bool IsDepthFormat(VkFormat format);
            bool IsStencilFormat(VkFormat format);
            VkPipelineStageFlags LayoutToAccessMask(const VkImageLayout layout, const bool isDestination);
            VkPipelineStageFlags AccessFlagsToPipelineStage(VkAccessFlags accessFlags, const VkPipelineStageFlags stageFlags);
            void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
                                       uint32_t mipLevels, uint32_t layerCount, VkCommandBuffer commandBuffer,
                                       VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
        }
    }
}

#define VK_CHECK_RESULT(f)                                                                                                                              \
    {                                                                                                                                                   \
        VkResult res = (f);                                                                                                                             \
        if (res != VK_SUCCESS)                                                                                                                          \
        {                                                                                                                                               \
            pluto::log<pluto::Error>("VkResult is %s in %s at line %s", pluto::Graphics::VKUtilities::GetErrorString(res).c_str(), __FILE__, __LINE__); \
        }                                                                                                                                               \
    }
