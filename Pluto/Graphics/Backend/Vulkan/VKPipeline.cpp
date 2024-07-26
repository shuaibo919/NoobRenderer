/* Vulkan Pipeline */
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Usage */

/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;
VKPipeline::VKPipeline(RenderContext *ctx, VKPipeline::Properties *&&pProperties)
    : Pipeline(ctx, std::move(pProperties))
{
}

VKPipeline::~VKPipeline()
{
}

void VKPipeline::BindVertexArray(SharedPtr<VertexBuffer> vbo)
{
}

void VKPipeline::Preparation()
{
}

void VKPipeline::Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer)
{
}

void VKPipeline::End(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void VKPipeline::ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer)
{
}
