/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKVertexBuffer::VKVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties)
    : VertexBuffer(ctx, std::move(pProperties))
{
}

VKVertexBuffer::~VKVertexBuffer()
{
}

void VKVertexBuffer::Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding)
{
}

void VKVertexBuffer::Unbind()
{
}