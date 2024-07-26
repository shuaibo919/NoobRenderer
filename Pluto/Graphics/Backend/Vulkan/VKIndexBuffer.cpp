/* Vulkan IndexBuffer */
#include "Graphics/Backend/Vulkan/VKIndexBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VK.h"
#include "Graphics/Backend/Vulkan/VKDebug.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;

VKIndexBuffer::VKIndexBuffer(RenderContext *ctx, VKIndexBuffer::Properties *&&pProperties)
    : IndexBuffer(ctx, std::move(pProperties))
{
}

VKIndexBuffer::~VKIndexBuffer()
{
}

void VKIndexBuffer::Bind(CommandBuffer *commandBuffer) const
{
}

void VKIndexBuffer::Unbind() const
{
}
