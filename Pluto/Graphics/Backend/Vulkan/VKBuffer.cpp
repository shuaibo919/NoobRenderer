/* Vulkan Buffer */
#include "Graphics/Backend/Vulkan/VKBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKBuffer::VKBuffer(VKRenderContext *pContext, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data)
    : mContext(pContext), mSize(size)
{
    mAllocation = nullptr;
    this->Init(usage, memoryProperyFlags, size, data);
}

VKBuffer::VKBuffer(VKRenderContext *pContext)
    : mContext(pContext), mSize(0), mMemoryProperyFlags(0), mUsageFlags(0)
{
    mAllocation = nullptr;
}

VKBuffer::~VKBuffer()
{
    // PLog<PInfo>("%p VKBuffer::VKBuffer Destroy RefCount:%d", this, RefCount);
    this->Destroy();
}

void VKBuffer::Destroy()
{
    if (mBuffer == VK_NULL_HANDLE)
        return;

    if (mMappedBuffer)
    {
        this->Flush();
        this->UnMap();
    }
    vmaDestroyBuffer(mContext->GetBasedDevice()->GetAllocator(), mBuffer, mAllocation);
}

void VKBuffer::Init(VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperyFlags, uint32_t size, const void *data)
{
    auto pBasedDevice = mContext->GetBasedDevice();
    mSize = size;
    mUsageFlags = usage;
    mMemoryProperyFlags = memoryProperyFlags;
    mGPUOnlyMemory = (memoryProperyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0;
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (data == nullptr)
    {
        VmaAllocationCreateInfo vmaAllocInfo = {};
        vmaAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        vmaAllocInfo.flags = !mGPUOnlyMemory ? VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT : 0;
        vmaAllocInfo.preferredFlags = memoryProperyFlags;
        vmaCreateBuffer(pBasedDevice->GetAllocator(), &bufferCreateInfo, &vmaAllocInfo, &mBuffer, &mAllocation, nullptr);
        return;
    }
    VmaAllocationCreateInfo vmaAllocInfo = {};
    vmaAllocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    vmaAllocInfo.flags = !mGPUOnlyMemory ? VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT : 0;
    vmaAllocInfo.preferredFlags = memoryProperyFlags;

    VkBufferCreateInfo stagingBufferInfo = {};
    stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferInfo.size = size;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VkBuffer stagingBuffer;
    VmaAllocation stagingAlloc;

    vmaCreateBuffer(pBasedDevice->GetAllocator(), &stagingBufferInfo, &vmaAllocInfo, &stagingBuffer, &stagingAlloc, nullptr);

    uint8_t *destData = nullptr;
    {
        VkResult res = static_cast<VkResult>(vmaMapMemory(pBasedDevice->GetAllocator(), stagingAlloc, (void **)&destData));
        if (res != VK_SUCCESS)
        {
            PLog<PCritical>("%s Failed to map buffer", PLineInfo);
        }
        memcpy(destData, data, size);
        vmaUnmapMemory(pBasedDevice->GetAllocator(), stagingAlloc);
    }
    vmaAllocInfo.flags = mGPUOnlyMemory ? 0 : VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
    vmaAllocInfo.usage = mGPUOnlyMemory ? VMA_MEMORY_USAGE_GPU_ONLY : VMA_MEMORY_USAGE_AUTO;
    bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    vmaCreateBuffer(pBasedDevice->GetAllocator(), &bufferCreateInfo, &vmaAllocInfo, &mBuffer, &mAllocation, &mAllocationInfo);

    VkCommandBuffer commandBuffer = VKUtilities::BeginSingleTimeCommands(pBasedDevice->GetDevice(), pBasedDevice->GetCommandPool()->GetHandle());

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, stagingBuffer, mBuffer, 1, &copyRegion);

    VKUtilities::EndSingleTimeCommands(pBasedDevice->GetDevice(), pBasedDevice->GetCommandPool()->GetHandle(), pBasedDevice->GetGraphicsQueue(), commandBuffer);
    vmaDestroyBuffer(pBasedDevice->GetAllocator(), stagingBuffer, stagingAlloc);
}

void VKBuffer::SetData(uint32_t size, const void *data, bool useBarrier)
{
    this->Map(size, 0);
    memcpy(mMapped, data, size);
    this->UnMap();
    this->Flush(size, 0);

    if (useBarrier)
    {
        auto pCmdBuffer = std::static_pointer_cast<VKCommandBuffer>(mContext->GetSwapChain()->GetCurrentCommandBuffer());
        VkBufferMemoryBarrier bufferBarrier = {};
        bufferBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        bufferBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        bufferBarrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        bufferBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        bufferBarrier.buffer = mBuffer;
        bufferBarrier.offset = 0;
        bufferBarrier.size = VK_WHOLE_SIZE;

        vkCmdPipelineBarrier(pCmdBuffer->GetHandle(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 0, nullptr, 1, &bufferBarrier, 0, nullptr);
    }
}

void VKBuffer::Resize(uint32_t size, const void *data)
{
    auto usage = mUsageFlags;

    this->Destroy();
    this->Init(usage, mMemoryProperyFlags, size, data);
}

void VKBuffer::Map(VkDeviceSize size, VkDeviceSize offset)
{
    auto pBasedDevice = mContext->GetBasedDevice();
    VkResult res = static_cast<VkResult>(vmaMapMemory(pBasedDevice->GetAllocator(), mAllocation, (void **)&mMapped));

    if (res != VK_SUCCESS)
    {
        PLog<PError>("%s Failed to map buffer", PLineInfo);
    }
    mMappedBuffer = true;
    // RefCount++;
    // PLog<PInfo>("%p Mapcount:%d", this, RefCount);
}

void VKBuffer::UnMap()
{
    auto pBasedDevice = mContext->GetBasedDevice();
    if (mMappedBuffer)
    {
        vmaUnmapMemory(pBasedDevice->GetAllocator(), mAllocation);
        vmaFlushAllocation(pBasedDevice->GetAllocator(), mAllocation, 0, mSize);
        mMapped = nullptr;
    }
    mMappedBuffer = false;
    // RefCount--;
    // PLog<PInfo>("%p Mapcount:%d", this, RefCount);
}

void VKBuffer::Flush(VkDeviceSize size, VkDeviceSize offset)
{
    auto pBasedDevice = mContext->GetBasedDevice();
    vmaFlushAllocation(pBasedDevice->GetAllocator(), mAllocation, offset, size);
}

void VKBuffer::Invalidate(VkDeviceSize size, VkDeviceSize offset)
{
    auto pBasedDevice = mContext->GetBasedDevice();
    vmaInvalidateAllocation(pBasedDevice->GetAllocator(), mAllocation, offset, size);
}
