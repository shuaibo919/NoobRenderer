#include "Graphics/Backend/OpenGL/GLRenderPass.h"
// #include "Graphics/Backend/OpenGL/GLRenderer.h"
// #include "Graphics/Backend/OpenGL/GLFramebuffer.h"
// #include "Graphics/Backend/OpenGL/GLDebug.h"

#include "Graphics/Backend/OpenGL/GL.h"

using namespace pluto::Graphics;
GLRenderPass::GLRenderPass(GLRenderPass::Properties *&&pProperties)
    : RenderPass(std::move(pProperties))
{
}

GLRenderPass::~GLRenderPass()
{
}
void GLRenderPass::BeginRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer, float *clearColour, std::shared_ptr<Framebuffer> &frame, SubPassContents contents) const
{
}
void GLRenderPass::EndRenderPass(std::shared_ptr<CommandBuffer> &commandBuffer)
{
}
// GLRenderPass::GLRenderPass(const RenderPassDesc &renderPassDesc)
// {
//     Init(renderPassDesc);
// }

// GLRenderPass::~GLRenderPass()
// {
// }

// bool GLRenderPass::Init(const RenderPassDesc &renderPassDesc)
// {
//     m_Clear = renderPassDesc.clear;
//     m_ClearCount = renderPassDesc.attachmentCount;
//     return false;
// }

// void GLRenderPass::BeginRenderPass(CommandBuffer *commandBuffer, float *clearColour,
//                                    Framebuffer *frame, SubPassContents contents, uint32_t width, uint32_t height) const
// {
//     if (frame != nullptr)
//     {
//         ((GLFramebuffer *)frame)->Bind(width, height);
//         // frame->SetClearColour(clearColour);
//         GLCall(glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]));
//     }
//     else
//     {
//         GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
//         GLCall(glClearColor(clearColour[0], clearColour[1], clearColour[2], clearColour[3]));
//         GLCall(glViewport(0, 0, width, height));
//     }

//     if (m_Clear)
//         GLRenderer::ClearInternal(RENDERER_BUFFER_COLOUR | RENDERER_BUFFER_DEPTH | RENDERER_BUFFER_STENCIL);
// }

// void GLRenderPass::EndRenderPass(CommandBuffer *commandBuffer)
// {
//     GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
// }

// void GLRenderPass::Init()
// {
//     CreateFunc = CreateFuncGL;
// }

// RenderPass *GLRenderPass::CreateFuncGL(const RenderPassDesc &renderPassDesc)
// {
//     return new GLRenderPass(renderPassDesc);
// }
