/* OpenGL Pipeline */
#include "Graphics/Backend/OpenGL/GLPipeline.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLCommandCall.h"
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

void GLPipeline::BindVertexArray(SharedPtr<VertexBuffer> vbo)
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
    std::vector<SharedPtr<Texture>> attachments;
    auto pRenderContext = static_cast<GLRenderContext *>(mRenderContext);

    if (mProperties->swapchainTarget)
    {
        attachmentTypes.push_back(AttachmentType::Color);
        attachments.push_back(nullptr);
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
        for (uint32_t i = 0; i < pRenderContext->GetSwapChain()->GetSwapChainBufferCount(); i++)
        {
            frameBufferProperties->screenUse = true;
            attachments[0] = pRenderContext->GetSwapChain()->GetImage(i);
            frameBufferProperties->attachments = attachments;

            mFramebuffers.emplace_back(
                OpenGL::CreateFrameBuffer(this->mRenderContext, new Framebuffer::Properties(*frameBufferProperties)));
        }
    }
    else
    {
        frameBufferProperties->attachments = attachments;
        frameBufferProperties->screenUse = false;
        frameBufferProperties->layer = 0;
        mFramebuffers.emplace_back(
            OpenGL::CreateFrameBuffer(this->mRenderContext, std::move(frameBufferProperties)));
        frameBufferProperties = nullptr;
    }
    delete frameBufferProperties;
}
void GLPipeline::Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer)
{
    auto pRenderContext = static_cast<GLRenderContext *>(mRenderContext);
    Framebuffer::Ptr framebuffer{nullptr};
    if (mProperties->swapchainTarget)
    {
        framebuffer = mFramebuffers[pRenderContext->GetSwapChain()->GetCurrentBufferIndex()];
    }
    else
    {
        framebuffer = mFramebuffers[0];
    }

    mRenderPass->BeginRenderPass(commandBuffer, mProperties->clearColor, framebuffer, Graphics::Inline);
    // if (mProperties->shader != nullptr)
    // {
    //     OpenGL::EmulateCmdRecording(commandBuffer, GlCmdWrap(mProperties->shader->Bind()));
    // }

    // auto update_gl_state = [&]()
    // {
    //     update_state(pRenderContext->state.EnableBlend, mProperties->transparencyEnabled,
    //                  FuncWrapper(glEnable(GL_BLEND)), FuncWrapper(glDisable(GL_BLEND)));

    //     if (pRenderContext->state.EnableBlend)
    //     {
    //         update_state(pRenderContext->state.CtxBlendMode, mProperties->blendMode, FuncWrapper(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)));
    //         update_state(pRenderContext->state.CtxBlendMode, mProperties->blendMode, FuncWrapper(glBlendFunc(GL_ZERO, GL_SRC_COLOR)));
    //         update_state(pRenderContext->state.CtxBlendMode, mProperties->blendMode, FuncWrapper(glBlendFunc(GL_ONE, GL_ZERO)));
    //         update_state(pRenderContext->state.CtxBlendMode, mProperties->blendMode, FuncWrapper(glBlendFunc(GL_NONE, GL_NONE)));
    //     }

    //     update_state(pRenderContext->state.EnableCull, mProperties->cullMode != CullMode::None, FuncWrapper(glEnable(GL_CULL_FACE)), FuncWrapper(glDisable(GL_CULL_FACE)));

    //     if (pRenderContext->state.EnableCull)
    //     {
    //         update_state(pRenderContext->state.CtxCullMode, mProperties->cullMode, FuncWrapper(glCullFace(GL_BACK)));
    //         update_state(pRenderContext->state.CtxCullMode, mProperties->cullMode, FuncWrapper(glCullFace(GL_FRONT)));
    //         update_state(pRenderContext->state.CtxCullMode, mProperties->cullMode, FuncWrapper(glCullFace(GL_FRONT_AND_BACK)));
    //     }

    //     update_state(pRenderContext->state.EnableDepthTest, mProperties->DepthTest, FuncWrapper(glEnable(GL_DEPTH_TEST)), FuncWrapper(glDisable(GL_DEPTH_TEST)));

    //     // todo more states

    //     if (mProperties->lineWidth != 1.0f)
    //         glLineWidth(mProperties->lineWidth);
    // };
    // OpenGL::EmulateCmdRecording(commandBuffer, GlCmdWrap(update_gl_state()));
}
void GLPipeline::End(const SharedPtr<CommandBuffer> &commandBuffer)
{
    mRenderPass->EndRenderPass(commandBuffer);

    // if (mProperties->lineWidth != 1.0f)
    //     glLineWidth(1.0f);
}
void GLPipeline::ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer)
{
    auto pRenderContext = static_cast<GLRenderContext *>(mRenderContext);
    for (auto &framebuffer : mFramebuffers)
    {
        static_cast<GLFramebuffer *>(framebuffer.get())->Bind(commandBuffer);
        pRenderContext->Clear(commandBuffer, RenderBufferColor | RenderBufferDepth | RenderBufferStencil);
    }
}
