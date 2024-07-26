/* Vulkan RenderPass */
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VK.h"
#include "Graphics/Backend/Vulkan/VKDebug.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKRenderPass::VKRenderPass(RenderContext *ctx, VKRenderPass::Properties *&&pProperties)
    : RenderPass(ctx, std::move(pProperties))
{
}

VKRenderPass::~VKRenderPass()
{
}

void VKRenderPass::BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const
{
}

void VKRenderPass::EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer)
{
}