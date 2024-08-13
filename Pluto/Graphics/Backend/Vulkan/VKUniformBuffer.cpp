/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKBuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKUniformBuffer::VKUniformBuffer(RenderContext *ctx, UniformBuffer::Properties *&&pProperties)
    : UniformBuffer(ctx, std::move(pProperties))
{
    if (mProperties->data == nullptr) // safe guard
        mProperties->size = 0;
    mBuffer = new VKBuffer(static_cast<VKRenderContext *>(ctx), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                           mProperties->size, mProperties->data);
}

VKUniformBuffer::~VKUniformBuffer()
{
    mBuffer->Flush();
    mBuffer->UnMap();
    mBuffer->Destroy();
    delete mBuffer;
}

void VKUniformBuffer::ReInit(uint32_t size, void *data)
{
    mProperties->size = size;
    mProperties->data = data;
    mBuffer->Flush();
    mBuffer->UnMap();
    mBuffer->Destroy();
    delete mBuffer;
    mBuffer = new VKBuffer(static_cast<VKRenderContext *>(mRenderContext), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                           mProperties->size, mProperties->data);
}

void VKUniformBuffer::SetData(uint32_t size, void *data)
{
    mBuffer->Map();
    mBuffer->SetData(size, data);
    mBuffer->UnMap();
}

void VKUniformBuffer::SetDynamicData(uint32_t size, uint32_t typeSize, void *data)
{
    mBuffer->Map();
    mBuffer->SetData(size, data);
    mBuffer->Flush();
    mBuffer->UnMap();
}