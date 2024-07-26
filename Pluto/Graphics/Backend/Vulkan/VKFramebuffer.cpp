/* Vulkan Framebuffer */
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VK.h"
#include "Graphics/Backend/Vulkan/VKDebug.h"
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
GLFramebuffer::GLFramebuffer(RenderContext *ctx, GLFramebuffer::Properties *&&pProperties)
    : Framebuffer(ctx, std::move(pProperties)), mColorAttachmentCount(0)
{
}

GLFramebuffer::~GLFramebuffer()
{
}

void GLFramebuffer::Bind()
{
}

void GLFramebuffer::UnBind()
{
}

void GLFramebuffer::Validate()
{
}

void GLFramebuffer::SetClearColor(const glm::vec4 &color)
{
}