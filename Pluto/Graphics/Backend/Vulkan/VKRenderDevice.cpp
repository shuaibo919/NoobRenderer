#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKCommandPool.hpp"
using namespace pluto;
using namespace pluto::Graphics;

namespace pluto::Graphics
{
    class VKPhysicalDevice
    {
    public:
        VKPhysicalDevice(const SharedPtr<VKContext> &pContext)
            : mBasedContext(pContext)
        {
            auto vkInstance = mBasedContext->GetVKInstance();
            vkEnumeratePhysicalDevices(vkInstance, &mGPUCount, VK_NULL_HANDLE);
            if (mGPUCount == 0)
            {
                log<Error>("Failed to find GPUs!");
            }

            std::vector<VkPhysicalDevice> physicalDevices(mGPUCount);
            vkEnumeratePhysicalDevices(vkInstance, &mGPUCount, physicalDevices.data());

            // First select the last gpu
            mHandle = physicalDevices.back();

            std::vector<PhysicalDeviceInfo> deviceInfos;
            for (VkPhysicalDevice physicalDevice : physicalDevices)
            {
                deviceInfos.push_back(GetInfo(physicalDevice));
            }

            std::sort(deviceInfos.begin(), deviceInfos.end(), [](PhysicalDeviceInfo &deviceA, PhysicalDeviceInfo &deviceB)
                      {
                    if(deviceA.Type == deviceB.Type)
                        return deviceA.MemorySize > deviceB.MemorySize;

                    return deviceA.Type < deviceB.Type; });

            mHandle = deviceInfos[0].Handle;
            mDeviceInfo = deviceInfos[0];

            vkGetPhysicalDeviceProperties(mHandle, &mPhysicalDeviceProperties);
            vkGetPhysicalDeviceMemoryProperties(mHandle, &mMemoryProperties);

            uint32_t queueFamilyCount;
            vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, nullptr);

            mQueueFamilyProperties.resize(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, mQueueFamilyProperties.data());

            uint32_t extCount = 0;
            vkEnumerateDeviceExtensionProperties(mHandle, nullptr, &extCount, nullptr);
            if (extCount > 0)
            {
                std::vector<VkExtensionProperties> extensions(extCount);
                if (vkEnumerateDeviceExtensionProperties(mHandle, nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
                {
                    log<Info>("Found {%d} device extensions", extensions.size());
                    for (const auto &ext : extensions)
                    {
                        mSupportedExtensions[ext.extensionName] = true;
                    }
                }
            }

            static const float defaultQueuePriority(0.0f);

            int requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
            mQueueFamilyIndices = GetQueueFamilyIndices(requestedQueueTypes);

            if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
            {
                VkDeviceQueueCreateInfo queueInfo = {};
                queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = mQueueFamilyIndices.Graphics;
                queueInfo.queueCount = 1;
                queueInfo.pQueuePriorities = &defaultQueuePriority;
                mQueueCreateInfos.push_back(queueInfo);
            }

            if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
            {
                if (mQueueFamilyIndices.Compute != mQueueFamilyIndices.Graphics)
                {
                    // If compute family index differs, we need an additional queue create info for the compute queue
                    VkDeviceQueueCreateInfo queueInfo = {};
                    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueInfo.queueFamilyIndex = mQueueFamilyIndices.Compute;
                    queueInfo.queueCount = 1;
                    queueInfo.pQueuePriorities = &defaultQueuePriority;
                    mQueueCreateInfos.push_back(queueInfo);
                }
            }

            if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
            {
                if ((mQueueFamilyIndices.Transfer != mQueueFamilyIndices.Graphics) && (mQueueFamilyIndices.Transfer != mQueueFamilyIndices.Compute))
                {
                    // If compute family index differs, we need an additional queue create info for the compute queue
                    VkDeviceQueueCreateInfo queueInfo{};
                    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queueInfo.queueFamilyIndex = mQueueFamilyIndices.Transfer;
                    queueInfo.queueCount = 1;
                    queueInfo.pQueuePriorities = &defaultQueuePriority;
                    mQueueCreateInfos.push_back(queueInfo);
                }
            }
        }
        ~VKPhysicalDevice()
        {
        }

        struct PhysicalDeviceInfo
        {
            std::string Name;
            PhysicalDeviceType Type;
            uint32_t MemorySize;
            uint32_t VendorID;
            VkPhysicalDevice Handle;
        };

        bool IsExtensionSupported(std::string extensionName) const
        {
            return mSupportedExtensions.find(extensionName) != mSupportedExtensions.end();
        }

        uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties) const
        {
            // Iterate over all memory types available for the device used in this example
            for (uint32_t i = 0; i < mMemoryProperties.memoryTypeCount; i++)
            {
                if ((typeBits & 1) == 1)
                {
                    if ((mMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                        return i;
                }
                typeBits >>= 1;
            }
            log<Error>("Could not find a suitable memory type!");
            return UINT32_MAX;
        }

        PhysicalDeviceInfo GetInfo(VkPhysicalDevice device)
        {
            VkPhysicalDeviceProperties properties = {};
            vkGetPhysicalDeviceProperties(device, &properties);

            VkPhysicalDeviceMemoryProperties memoryProperties = {};
            vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

            PhysicalDeviceType type = PhysicalDeviceType::Unknown;
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                type = PhysicalDeviceType::Discrete;
            else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                type = PhysicalDeviceType::Integrated;
            else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
                type = PhysicalDeviceType::Virtual;
            else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
                type = PhysicalDeviceType::Cpu;

            uint64_t memory = static_cast<uint64_t>(memoryProperties.memoryHeaps[0].size);
            uint32_t memoryMB = static_cast<uint32_t>(memory / 1024 / 1024);

            VKPhysicalDevice::PhysicalDeviceInfo info = {};
            info.Name = std::string(properties.deviceName);
            info.VendorID = properties.vendorID;
            info.MemorySize = memoryMB;
            info.Type = type;
            info.Handle = device;

            return info;
        }

        VkPhysicalDevice GetHandle() const
        {
            return mHandle;
        }

        int32_t GetGraphicsQueueFamilyIndex()
        {
            return mQueueFamilyIndices.Graphics;
        }

        VkPhysicalDeviceProperties GetProperties() const
        {
            return mPhysicalDeviceProperties;
        }

        VkPhysicalDeviceMemoryProperties GetMemoryProperties() const
        {
            return mMemoryProperties;
        }

    private:
        struct QueueFamilyIndices
        {
            int32_t Graphics = -1;
            int32_t Compute = -1;
            int32_t Transfer = -1;
        };
        QueueFamilyIndices GetQueueFamilyIndices(int flags)
        {
            QueueFamilyIndices indices;

            // Dedicated queue for compute
            // Try to find a queue family index that supports compute but not graphics
            if (flags & VK_QUEUE_COMPUTE_BIT)
            {
                for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++)
                {
                    auto &queueFamilyProperties = mQueueFamilyProperties[i];
                    if ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
                    {
                        indices.Compute = i;
                        break;
                    }
                }
            }

            // Dedicated queue for transfer
            // Try to find a queue family index that supports transfer but not graphics and compute
            if (flags & VK_QUEUE_TRANSFER_BIT)
            {
                for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++)
                {
                    auto &queueFamilyProperties = mQueueFamilyProperties[i];
                    if ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
                    {
                        indices.Transfer = i;
                        break;
                    }
                }
            }

            // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
            for (uint32_t i = 0; i < mQueueFamilyProperties.size(); i++)
            {
                if ((flags & VK_QUEUE_TRANSFER_BIT) && indices.Transfer == -1)
                {
                    if (mQueueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                        indices.Transfer = i;
                }

                if ((flags & VK_QUEUE_COMPUTE_BIT) && indices.Compute == -1)
                {
                    if (mQueueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                        indices.Compute = i;
                }

                if (flags & VK_QUEUE_GRAPHICS_BIT)
                {
                    if (mQueueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                        indices.Graphics = i;
                }
            }

            return indices;
        }

        uint32_t GetGPUCount() const
        {
            return mGPUCount;
        }

    private:
        SharedPtr<VKContext> mBasedContext;
        std::unordered_map<std::string, bool> mSupportedExtensions;

        QueueFamilyIndices mQueueFamilyIndices;
        std::vector<VkQueueFamilyProperties> mQueueFamilyProperties;
        std::vector<VkDeviceQueueCreateInfo> mQueueCreateInfos;

        VkPhysicalDevice mHandle;
        VkPhysicalDeviceProperties mPhysicalDeviceProperties;
        VkPhysicalDeviceMemoryProperties mMemoryProperties;

        uint32_t mGPUCount{0};
        PhysicalDeviceInfo mDeviceInfo;

        friend class VKRenderDevice;
    };
}

VKRenderDevice::VKRenderDevice(const SharedPtr<GraphicsContext> &pContext)
{
    mBasedContext = std::static_pointer_cast<VKContext>(pContext);
    mPhysicalDevice = new VKPhysicalDevice(mBasedContext);

    VkPhysicalDeviceFeatures supportedFeatures;
    memset(&supportedFeatures, 0, sizeof(VkPhysicalDeviceFeatures));
    memset(&mEnabledFeatures, 0, sizeof(VkPhysicalDeviceFeatures));

    vkGetPhysicalDeviceFeatures(mPhysicalDevice->GetHandle(), &supportedFeatures);

    if (supportedFeatures.samplerAnisotropy)
        mEnabledFeatures.samplerAnisotropy = true;

    if (supportedFeatures.depthClamp)
        mEnabledFeatures.depthClamp = true;

    if (supportedFeatures.depthBiasClamp)
        mEnabledFeatures.depthBiasClamp = true;

    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    // The Vulkan spec states: If the VK_KHR_portability_subset extension is included in pProperties of vkEnumerateDeviceExtensionProperties,
    // ppEnabledExtensionNames must include "VK_KHR_portability_subset"
    // (https://vulkan.lunarg.com/doc/view/1.3.283.0/mac/1.3-extensions/vkspec.html#VUID-VkDeviceCreateInfo-pProperties-04451)}
    if (mPhysicalDevice->IsExtensionSupported("VK_KHR_portability_subset"))
    {
        deviceExtensions.push_back("VK_KHR_portability_subset");
        log<Info>("VK_KHR_portability_subset extension loaded");
    }

    VkPhysicalDeviceDescriptorIndexingFeaturesEXT indexingFeatures = {};
    indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES_EXT;
    indexingFeatures.runtimeDescriptorArray = VK_TRUE;
    indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;

    // Device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(mPhysicalDevice->mQueueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = mPhysicalDevice->mQueueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &mEnabledFeatures;
    deviceCreateInfo.enabledLayerCount = 0;
    deviceCreateInfo.pNext = (void *)&indexingFeatures;

    auto result = vkCreateDevice(mPhysicalDevice->GetHandle(), &deviceCreateInfo, VK_NULL_HANDLE, &mDevice);
    if (result != VK_SUCCESS)
    {
        log<Info>("Failed to create device");
    }

    vkGetDeviceQueue(mDevice, mPhysicalDevice->mQueueFamilyIndices.Graphics, 0, &mGraphicsQueue);
    vkGetDeviceQueue(mDevice, mPhysicalDevice->mQueueFamilyIndices.Graphics, 0, &mPresentQueue);
    vkGetDeviceQueue(mDevice, mPhysicalDevice->mQueueFamilyIndices.Compute, 0, &mComputeQueue);

    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = mPhysicalDevice->GetHandle();
    allocatorInfo.device = mDevice;
    allocatorInfo.instance = mBasedContext->GetVKInstance();
    allocatorInfo.vulkanApiVersion = mBasedContext->GetVKVersion();

    allocatorInfo.preferredLargeHeapBlockSize = 64 * 1024 * 1024;
    if (vmaCreateAllocator(&allocatorInfo, &mAllocator) != VK_SUCCESS)
    {
        log<Error>("Failed to create VMA allocator");
    }

    mCommandPool = new VKCommandPool(mDevice, mPhysicalDevice->GetGraphicsQueueFamilyIndex(), VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    VkPipelineCacheCreateInfo pipelineCacheInfo = {};
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    pipelineCacheInfo.pNext = NULL;
    vkCreatePipelineCache(mDevice, &pipelineCacheInfo, VK_NULL_HANDLE, &mPipelineCache);
}

VKRenderDevice::~VKRenderDevice()
{
    if (mCommandPool != nullptr)
        delete mCommandPool;

    vkDestroyPipelineCache(mDevice, mPipelineCache, VK_NULL_HANDLE);

    for (auto &pool : mAllocPools)
    {
        vmaDestroyPool(mAllocator, pool.second);
    }
    mAllocPools.clear();

    vmaDestroyAllocator(mAllocator);

    vkDestroyDevice(mDevice, VK_NULL_HANDLE);
}

void VKRenderDevice::InitCreateImpl()
{
    RenderDevice::CreateImpl = CreateRenderDeviceVKImpl;
}

RenderDevice *VKRenderDevice::CreateRenderDeviceVKImpl(const SharedPtr<GraphicsContext> &pContext)
{
    return new VKRenderDevice(pContext);
}

uint32_t VKRenderDevice::GetGPUCount() const
{
    return mPhysicalDevice->GetGPUCount();
}

VkPhysicalDevice VKRenderDevice::GetGPU() const
{
    return mPhysicalDevice->GetHandle();
}

int32_t VKRenderDevice::GetGraphicsQueueFamilyIndex()
{
    return mPhysicalDevice->GetGraphicsQueueFamilyIndex();
}

VkPhysicalDeviceProperties VKRenderDevice::GetProperties() const
{
    return mPhysicalDevice->GetProperties();
}

bool VKRenderDevice::IsExtensionSupported(std::string extensionName) const
{
    return mPhysicalDevice->IsExtensionSupported(extensionName);
}

VkPhysicalDeviceMemoryProperties VKRenderDevice::GetMemoryProperties() const
{
    return mPhysicalDevice->GetMemoryProperties();
}

uint32_t VKRenderDevice::GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties) const
{
    return mPhysicalDevice->GetMemoryTypeIndex(typeBits, properties);
}
