#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"
using namespace pluto::Graphics;

VKSemaphore::VKSemaphore(VkDevice device, bool timeline)
    : mBasedDevice(device)
{
    mTimeline = timeline;

    VkSemaphoreTypeCreateInfo semaphoreTypeCreateInfo = {};
    semaphoreTypeCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
    semaphoreTypeCreateInfo.pNext = nullptr;
    semaphoreTypeCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
    semaphoreTypeCreateInfo.initialValue = 0;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = mTimeline ? &semaphoreTypeCreateInfo : nullptr;
    semaphoreInfo.flags = 0;

    VK_CHECK_RESULT(vkCreateSemaphore(mBasedDevice, &semaphoreInfo, nullptr, &mHandle));
}

VKSemaphore::~VKSemaphore()
{
    vkDestroySemaphore(mBasedDevice, mHandle, nullptr);
}

void VKSemaphore::Wait(uint64_t value, uint64_t timeout)
{
    VkSemaphoreWaitInfo semaphoreWaitInfo = {};
    semaphoreWaitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
    semaphoreWaitInfo.pNext = nullptr;
    semaphoreWaitInfo.flags = 0;
    semaphoreWaitInfo.semaphoreCount = 1;
    semaphoreWaitInfo.pSemaphores = &mHandle;
    semaphoreWaitInfo.pValues = &value;
}

void VKSemaphore::Signal(uint64_t value)
{
    VkSemaphoreSignalInfo semaphoreSignalInfo = {};
    semaphoreSignalInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SIGNAL_INFO;
    semaphoreSignalInfo.pNext = nullptr;
    semaphoreSignalInfo.semaphore = mHandle;
    semaphoreSignalInfo.value = value;
}

uint64_t VKSemaphore::GetValue()
{
    uint64_t value = 0;
    VK_CHECK_RESULT(vkGetSemaphoreCounterValue(mBasedDevice, mHandle, &value));
    return value;
}
