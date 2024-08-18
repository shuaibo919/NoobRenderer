/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKBuffer.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKVertexBuffer::VKVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties)
    : VertexBuffer(ctx, std::move(pProperties))
{
    mBuffer = new VKBuffer(static_cast<VKRenderContext *>(ctx), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                           pProperties->usage == BufferUsage::Dynamic ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0,
                           mProperties->vetexSize, mProperties->data);
}

VKVertexBuffer::~VKVertexBuffer()
{
    RHIBase::Destroy();
}

void VKVertexBuffer::DestroyImplementation()
{
    delete mBuffer;
}

void VKVertexBuffer::Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding)
{
    VkDeviceSize offsets[1] = {0};
    if (commandBuffer)
        vkCmdBindVertexBuffers(std::static_pointer_cast<VKCommandBuffer>(commandBuffer)->GetHandle(), binding, 1, &mBuffer->GetBuffer(), offsets);
}

void VKVertexBuffer::Unbind()
{
}