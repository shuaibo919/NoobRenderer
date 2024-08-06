#pragma once
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKSemaphore
        {
        public:
            VKSemaphore(VkDevice device, bool timeline = false);
            ~VKSemaphore();

            VkSemaphore &GetHandle() { return mHandle; }

            void Signal(uint64_t value);
            void Wait(uint64_t value, uint64_t timeout);

            uint64_t GetValue();

        private:
            VkDevice mBasedDevice;
            VkSemaphore mHandle;
            bool mTimeline;
        };
        class VKFence
        {
        public:
            VKFence(VkDevice device, bool signaled = false);
            ~VKFence();

            bool CheckState();
            bool IsSignaled();
            VkFence &GetHandle() { return mHandle; }

            bool Wait(uint64_t timeout = 1000);
            void Reset();
            void WaitAndReset();

        private:
            VkDevice mBasedDevice;
            VkFence mHandle;
            bool mSignaled;
        };
    }
}