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
