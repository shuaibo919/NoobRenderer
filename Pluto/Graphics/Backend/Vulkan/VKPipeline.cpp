/* Vulkan Pipeline */
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Usage */

/* Common */
#include "Graphics/Backend/Vulkan/VK.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
GLPipeline::GLPipeline(RenderContext *ctx, GLPipeline::Properties *&&pProperties)
    : Pipeline(ctx, std::move(pProperties))
{
}

GLPipeline::~GLPipeline()
{
}

void GLPipeline::BindVertexArray(SharedPtr<VertexBuffer> vbo)
{
}

void GLPipeline::Preparation()
{
}

void GLPipeline::Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer)
{
}

void GLPipeline::End(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void GLPipeline::ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer)
{
}
