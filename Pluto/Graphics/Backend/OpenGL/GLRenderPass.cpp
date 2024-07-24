/* OpenGL RenderPass */
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandCall.h"
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
        std::dynamic_pointer_cast<GLFramebuffer>(frame)->Bind(commandBuffer);
        OpenGL::EmulateCmdRecording(commandBuffer, GlCmd(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3])));
    }
    else
    {
        OpenGL::EmulateCmdRecording(commandBuffer, GlCmd(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0)));
        OpenGL::EmulateCmdRecording(commandBuffer, GlCmd(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3])));
    }

    if (mProperties->clear)
    {
        static_cast<GLRenderContext *>(mRenderContext)->Clear(commandBuffer, RendererBufferType::RenderBufferColor | RendererBufferType::RenderBufferDepth | RendererBufferType::RenderBufferStencil);
    }
}
void GLRenderPass::EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer)
{
    OpenGL::EmulateCmdRecording(commandBuffer, GlCmd(glBindFramebuffer(GL_FRAMEBUFFER, 0)));
}