#pragma once
#include "Graphics/RHI/RenderDevice.h"
#include "Graphics/RHI/Declarations.h"

#include "Graphics/Backend/Vulkan/Vk.h"
#include "Graphics/Backend/Vulkan/VKCommandPool.hpp"

namespace pluto
{
    namespace Graphics
    {
        class VKContext;
        class VKSwapChain;
        class VKPhysicalDevice;
        class VKRenderDevice : public RenderDevice
        {
            friend class VKSwapChain;

        public:
            VKRenderDevice(const SharedPtr<GraphicsContext> &pContext);
            ~VKRenderDevice();

            static void InitCreateImpl();

        public:
            uint32_t GetGPUCount() const;
            VkPhysicalDevice GetGPU() const;
            const VkPhysicalDeviceFeatures &GetEnabledFeatures() { return mEnabledFeatures; }
            VmaAllocator GetAllocator() const { return mAllocator; }
            VkDevice GetDevice() const { return mDevice; }
            VkQueue GetGraphicsQueue() const { return mGraphicsQueue; }
            VkQueue GetPresentQueue() const { return mPresentQueue; }
            VkQueue GetComputeQueue() const { return mComputeQueue; }
            VKCommandPool const *GetCommandPool() const { return mCommandPool; }
            VkPipelineCache GetPipelineCache() const { return mPipelineCache; }

        protected:
            static RenderDevice *CreateRenderDeviceVKImpl(const SharedPtr<GraphicsContext> &pContext);

        protected:
            /* protected caps of the physical device */
            int32_t GetGraphicsQueueFamilyIndex();
            VkPhysicalDeviceProperties GetProperties() const;
            bool IsExtensionSupported(std::string extensionName) const;
            VkPhysicalDeviceMemoryProperties GetMemoryProperties() const;
            uint32_t GetMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties) const;

        private:
            SharedPtr<VKContext> mBasedContext;
            VkDevice mDevice;
            VmaAllocator mAllocator{};
            std::unordered_map<uint32_t, VmaPool> mAllocPools;

            VkQueue mComputeQueue;
            VkQueue mGraphicsQueue;
            VkQueue mPresentQueue;
            VkPipelineCache mPipelineCache;
            VkDescriptorPool mDescriptorPool;
            VkPhysicalDeviceFeatures mEnabledFeatures;

            VKCommandPool *mCommandPool;
            VKPhysicalDevice *mPhysicalDevice;

            bool mEnableDebugMarkers = false;

            uint32_t mGraphicsQueueFamilyIndex;
        };
    }
}
