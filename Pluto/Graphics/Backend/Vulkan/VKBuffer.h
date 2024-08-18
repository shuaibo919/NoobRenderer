#pragma once
#include "Graphics/Backend/Vulkan/Vk.h"

namespace pluto
{
    namespace Graphics
    {
        class VKRenderContext;
        class VKBuffer
        {
        public:
            VKBuffer(VKRenderContext *pContext, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data);
            VKBuffer(VKRenderContext *pContext);
            virtual ~VKBuffer();

            void Init(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data);
            void Resize(uint32_t size, const void *data);
            void SetData(uint32_t size, const void *data, bool useBarrier = false);
            const VkBuffer &GetBuffer() const { return mBuffer; }

            const VkDescriptorBufferInfo &GetBufferInfo() const { return mDesciptorBufferInfo; };

            void Map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void UnMap();
            void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
            void SetUsage(VkBufferUsageFlags flags) { mUsageFlags = flags; }
            void SetMemoryProperyFlags(VkBufferUsageFlags flags) { mMemoryProperyFlags = flags; }

        protected:
            VKRenderContext *mContext;

            VkBuffer mBuffer{};
            VkDeviceMemory mMemory{};
            VkDescriptorBufferInfo mDesciptorBufferInfo{};
            VkDeviceSize mSize{0};
            VkDeviceSize mAlignment{0};
            VkBufferUsageFlags mUsageFlags;
            VkMemoryPropertyFlags mMemoryProperyFlags;
            void *mMapped{nullptr};
            bool mGPUOnlyMemory{false};
            bool mMappedBuffer{false};

            VmaAllocation mAllocation;
            VmaAllocation mMappedAllocation;
            VmaAllocationInfo mAllocationInfo;

            void Destroy();
        };

    }
}
