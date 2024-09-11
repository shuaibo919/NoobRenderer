/* Vulkan IndexBuffer */
#include "Graphics/Backend/Vulkan/VKIndexBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKIndexBuffer::VKIndexBuffer(RenderContext *ctx, VKIndexBuffer::Properties *&&pProperties)
    : IndexBuffer(ctx, std::move(pProperties)), VKObjectManageByContext(static_cast<VKRenderContext *>(ctx))
{
}

VKIndexBuffer::~VKIndexBuffer()
{
    VKObjectManageByContext::Destroy();
}

void VKIndexBuffer::Bind(CommandBuffer *commandBuffer) const
{
}

void VKIndexBuffer::Unbind() const
{
}

void VKIndexBuffer::DestroyImplementation()
{
}