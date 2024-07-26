/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKShader.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKUniformBuffer::VKUniformBuffer(RenderContext *ctx, UniformBuffer::Properties *&&pProperties)
    : UniformBuffer(ctx, std::move(pProperties))
{
}

VKUniformBuffer::~VKUniformBuffer()
{
}

void VKUniformBuffer::ReInit(uint32_t size, void *data)
{
}

void VKUniformBuffer::SetData(uint32_t size, void *data)
{
}

void VKUniformBuffer::SetDynamicData(uint32_t size, uint32_t typeSize, void *data)
{
}