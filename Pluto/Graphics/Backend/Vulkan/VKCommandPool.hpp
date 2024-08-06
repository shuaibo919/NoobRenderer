#pragma once
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKCommandPool
        {
        public:
            VKCommandPool(VkDevice device, int queueIndex, VkCommandPoolCreateFlags flags)
                : mBasedDevice(device)
            {
                VkCommandPoolCreateInfo cmdPoolCI{};
                cmdPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
                cmdPoolCI.queueFamilyIndex = queueIndex;
                cmdPoolCI.flags = flags;

                vkCreateCommandPool(mBasedDevice, &cmdPoolCI, nullptr, &mHandle);
            }
            ~VKCommandPool()
            {
                vkDestroyCommandPool(mBasedDevice, mHandle, nullptr);
            }

            void Reset()
            {
                vkResetCommandPool(mBasedDevice, mHandle, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
            }

            const VkCommandPool &GetHandle() const { return mHandle; }

        private:
            VkCommandPool mHandle;
            VkDevice mBasedDevice;
        };

    }
}
