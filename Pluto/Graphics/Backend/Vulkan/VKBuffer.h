#pragma once
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKBuffer
        {
        public:
            VKBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data);
            VKBuffer();
            virtual ~VKBuffer();

            void Init(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data);
            void Resize(uint32_t size, const void *data);
            void SetData(uint32_t size, const void *data, bool addBarrier = false);
            const VkBuffer &GetBuffer() const { return mBuffer; }

            const VkDescriptorBufferInfo &GetBufferInfo() const { return mDesciptorBufferInfo; };

            void Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void UnMap();
            void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void SetUsage(VkBufferUsageFlags flags) { mUsageFlags = flags; }
            void SetMemoryProperyFlags(VkBufferUsageFlags flags) { mMemoryProperyFlags = flags; }
            void Destroy(bool deletionQueue = false);
            void SetDeleteWithoutQueue(bool value) { mDeleteWithoutQueue = value; }

        protected:
            VkBuffer mBuffer{};
            VkDeviceMemory mMemory{};
            VkDescriptorBufferInfo mDesciptorBufferInfo{};
            VkDeviceSize mSize{0};
            VkDeviceSize mAlignment{0};
            VkBufferUsageFlags mUsageFlags;
            VkMemoryPropertyFlags mMemoryProperyFlags;
            void *mMapped{nullptr};
            bool mDeleteWithoutQueue{false};
            bool mGPUOnlyMemory{false};

            VmaAllocation mAllocation;
            VmaAllocation mMappedAllocation;
            VmaAllocationInfo mAllocationInfo;
        };

    }
}
