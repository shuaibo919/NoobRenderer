/* OpenGL RenderPass */
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLRenderPass::GLRenderPass(GLRenderPass::Properties *&&pProperties)
    : RenderPass(std::move(pProperties))
{
}

GLRenderPass::~GLRenderPass()
{
}

void GLRenderPass::BeginRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer, float *clearColor, std::shared_ptr<Framebuffer> &frame, SubPassContents contents) const
{
    if (frame != nullptr)
    {
        std::dynamic_pointer_cast<GLFramebuffer>(frame)->Bind();
        GlCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
    }
    else
    {
        GlCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        GlCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]));
    }

    if (mProperties->clear)
    {
        // reset state
        // :todo
    }
}
void GLRenderPass::EndRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer)
{
    GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}