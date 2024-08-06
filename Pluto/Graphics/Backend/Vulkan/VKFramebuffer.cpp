/* Vulkan Framebuffer */
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKFramebuffer::VKFramebuffer(RenderContext *ctx, VKFramebuffer::Properties *&&pProperties)
    : Framebuffer(ctx, std::move(pProperties))
{
}

VKFramebuffer::~VKFramebuffer()
{
}

void VKFramebuffer::Validate()
{
}

void VKFramebuffer::SetClearColor(const glm::vec4 &color)
{
}