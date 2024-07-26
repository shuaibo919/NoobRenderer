/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Common */
#include "Graphics/Backend/Vulkan/VK.h"
#include "Graphics/Backend/Vulkan/VKDebug.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
GLVertexBuffer::GLVertexBuffer(RenderContext *ctx, VertexBuffer::Properties *&&pProperties)
    : VertexBuffer(ctx, std::move(pProperties))
{
}

GLVertexBuffer::~GLVertexBuffer()
{
}

void GLVertexBuffer::Bind(const SharedPtr<CommandBuffer> &commandBuffer, const SharedPtr<Pipeline> &pipeline, uint8_t binding)
{
}

void GLVertexBuffer::Unbind()
{
}