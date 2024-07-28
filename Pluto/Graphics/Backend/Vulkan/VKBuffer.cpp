/* Vulkan Buffer */
#include "Graphics/Backend/Vulkan/VKBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKBuffer::VKBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data)
    : mSize(size)
{
    Init(usage, memoryProperyFlags, size, data);
}

VKBuffer::VKBuffer()
    : mSize(0), mMemoryProperyFlags(0), mUsageFlags(0)
{
}

VKBuffer::~VKBuffer()
{
}

void VKBuffer::Destroy(bool deletionQueue)
{
}

void VKBuffer::Init(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data)
{
}

void VKBuffer::SetData(uint32_t size, const void *data, bool addBarrier)
{
}

void VKBuffer::Resize(uint32_t size, const void *data)
{
}

void VKBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
{
}

void VKBuffer::UnMap()
{
}

void VKBuffer::Flush(VkDeviceSize size, VkDeviceSize offset)
{
}

void VKBuffer::Invalidate(VkDeviceSize size, VkDeviceSize offset)
{
}
