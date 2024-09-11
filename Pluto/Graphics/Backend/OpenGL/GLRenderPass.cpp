/* OpenGL RenderPass */
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLRenderPass::GLRenderPass(RenderContext *ctx, GLRenderPass::Properties *&&pProperties)
    : RenderPass(ctx, std::move(pProperties))
{
}

GLRenderPass::~GLRenderPass()
{
}

void GLRenderPass::BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const
{
    if (frame != nullptr)
    {
        std::dynamic_pointer_cast<GLFramebuffer>(frame)->Bind();
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    }
    else
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    }

    if (mProperties->clear)
    {
        GLRenderContext::Clear(RendererBufferType::RenderBufferColor | RendererBufferType::RenderBufferDepth | RendererBufferType::RenderBufferStencil);
    }
}
void GLRenderPass::EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}