/* OpenGL Pipeline */
#include "Graphics/Backend/OpenGL/GLPipeline.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLPipeline::GLPipeline(RenderContext *ctx, GLPipeline::Properties *&&pProperties)
    : Pipeline(ctx, std::move(pProperties))
{
    glGenVertexArrays(1, &mVertexArray);
    this->Preparation();
}

GLPipeline::~GLPipeline()
{
    glDeleteVertexArrays(1, &mVertexArray);
}
void GLPipeline::BindVertexArray(std::shared_ptr<VertexBuffer> vbo)
{
    GlCall(glBindVertexArray(static_cast<GLuint>(mVertexArray)));
    auto &vbo_propertis = vbo->GetProperties();
    uint16_t attriArrayPosition = 0;
    for (size_t i = 0; i < vbo_propertis.usedAttributes.size(); ++i)
    {
        if (vbo_propertis.usedAttributes[i])
        {
            GLuint index = static_cast<GLuint>(i);
            GLint count = GLUtilities::GetElementCount(vbo_propertis.attributes[i].type);
            GLenum type = GLUtilities::GetDataType(vbo_propertis.attributes[i].type);
            GLsizei stride = vbo_propertis.attributes[i].stride;
            void const *pointer = reinterpret_cast<void const *>(vbo_propertis.attributes[i].offset);

            GlCall(glEnableVertexAttribArray(attriArrayPosition));
            GlCall(glVertexAttribPointer(attriArrayPosition, count, type, GL_FALSE, stride, pointer));
            attriArrayPosition++;
        }
    }
}
void GLPipeline::Preparation()
{
    std::vector<AttachmentType> attachmentTypes;
    std::vector<std::shared_ptr<Texture>> attachments;

    if (mProperties->swapchainTarget)
    {
        log<Critical>("Not Implemented");
        // attachmentTypes.push_back(AttachmentType::Color);
    }
    else
    {
        for (auto i = 0; i < mProperties->attachmentTypes.size(); ++i)
        {
            if (mProperties->colorTargets[i] != nullptr)
            {
                attachmentTypes.push_back(mProperties->attachmentTypes[i]);
                attachments.push_back(mProperties->colorTargets[i]);
            }
        }
    }

    auto renderPassProperties = new RenderPass::Properties();
    renderPassProperties->attachments = attachments;
    renderPassProperties->attachmentTypes = attachmentTypes;
    renderPassProperties->clear = mProperties->clearTargets;
    renderPassProperties->mipIndex = mProperties->mipIndex;
    mRenderPass = std::dynamic_pointer_cast<GLRenderPass>(OpenGL::CreateRenderPass(this->mRenderContext, std::move(renderPassProperties)));

    auto frameBufferProperties = new Framebuffer::Properties();
    frameBufferProperties->width = this->GetWidth();
    frameBufferProperties->height = this->GetHeight();
    frameBufferProperties->renderPass = mRenderPass;
    frameBufferProperties->mipIndex = mProperties->mipIndex;
    frameBufferProperties->attachmentTypes = mProperties->attachmentTypes;
    if (mProperties->swapchainTarget)
    {
        log<Critical>("Not Implemented");
    }
    else
    {
        frameBufferProperties->attachments = attachments;
        frameBufferProperties->screenUse = false;
        frameBufferProperties->layer = 0;
        mFramebuffers.push_back(std::dynamic_pointer_cast<GLFramebuffer>(OpenGL::CreateFrameBuffer(this->mRenderContext, std::move(frameBufferProperties))));
    }
}
void GLPipeline::Bind(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t layer)
{
    Framebuffer::Ptr framebuffer;
    if (mProperties->swapchainTarget)
    {
        log<Critical>("Not Implemented");
    }
    else
    {
        framebuffer = mFramebuffers[0];
    }

    mRenderPass->BeginRenderPass(commandBuffer, mProperties->clearColor, framebuffer, Graphics::Inline);
    mProperties->shader->Bind();

    if (mProperties->transparencyEnabled)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        if (mProperties->blendMode == BlendMode::SrcAlphaOneMinusSrcAlpha)
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else if (mProperties->blendMode == BlendMode::ZeroSrcColor)
        {
            glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        }
        else if (mProperties->blendMode == BlendMode::OneZero)
        {
            glBlendFunc(GL_ONE, GL_ZERO);
        }
        else
        {
            glBlendFunc(GL_NONE, GL_NONE);
        }
    }
    else
        glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);

    switch (mProperties->cullMode)
    {
    case CullMode::Back:
        glCullFace(GL_BACK);
        break;
    case CullMode::Front:
        glCullFace(GL_FRONT);
        break;
    case CullMode::FrontAndBack:
        glCullFace(GL_FRONT_AND_BACK);
        break;
    case CullMode::None:
        glDisable(GL_CULL_FACE);
        break;
    }

    glFrontFace(GL_CCW);

    if (mProperties->lineWidth != 1.0f)
        glLineWidth(mProperties->lineWidth);
}
void GLPipeline::End(std::shared_ptr<CommandBuffer> commandBuffer)
{
    mRenderPass->EndRenderPass(commandBuffer);

    if (mProperties->lineWidth != 1.0f)
        glLineWidth(1.0f);
}
void GLPipeline::ClearRenderTargets(std::shared_ptr<CommandBuffer> commandBuffer)
{
    for (auto framebuffer : mFramebuffers)
    {
        // framebuffer.As<GLFramebuffer>()->Bind();
        // GLRenderer::ClearInternal(RenderBufferColor | RenderBufferDepth | RenderBufferStencil);
    }
}
