#include "Graphics/Backend/OpenGL/GLPipeline.h"
// #include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
// #include "Graphics/Backend/OpenGL/GLRenderer.h"
// #include "Graphics/Backend/OpenGL/GLDebug.h"

#include "Graphics/Backend/OpenGL/GL.h"

using namespace pluto::Graphics;
GLPipeline::GLPipeline(GLPipeline::Properties *&&pProperties)
    : Pipeline(std::move(pProperties))
{
}

GLPipeline::~GLPipeline()
{
}
void GLPipeline::BindVertexArray()
{
    //     GLCall(glBindVertexArray(m_VertexArray));
    //     auto &vertexLayout = ((GLShader *)mShader)->GetBufferLayout().GetLayout();
    //     uint32_t count = 0;

    //     for (auto &layout : vertexLayout)
    //     {
    //         GLCall(glEnableVertexAttribArray(count));
    //         size_t offset = static_cast<size_t>(layout.offset);
    //         VertexAtrribPointer(layout.format, count, offset, ((GLShader *)mShader)->GetBufferLayout().GetStride());
    //         count++;
    //     }
}
void GLPipeline::CreateFramebuffers()
{
    //     std::vector<TextureType> attachmentTypes;
    //     std::vector<Texture *> attachments;

    //     if (m_Description.swapchainTarget)
    //     {
    //         attachmentTypes.push_back(TextureType::COLOUR);
    //         attachments.push_back(Renderer::GetMainSwapChain()->GetImage(0));
    //     }
    //     else
    //     {
    //         // for (auto texture : m_Description.colourTargets)
    //         // {
    //         //     if (texture)
    //         //     {
    //         //         attachmentTypes.push_back(texture->GetType());
    //         //         attachments.push_back(texture);
    //         //     }
    //         // }
    //     }

    //     // if (m_Description.depthTarget)
    //     // {
    //     //     attachmentTypes.push_back(m_Description.depthTarget->GetType());
    //     //     attachments.push_back(m_Description.depthTarget);
    //     // }

    //     // if (m_Description.depthArrayTarget)
    //     // {
    //     //     attachmentTypes.push_back(m_Description.depthArrayTarget->GetType());
    //     //     attachments.push_back(m_Description.depthArrayTarget);
    //     // }

    //     // if (m_Description.cubeMapTarget)
    //     // {
    //     //     attachmentTypes.push_back(m_Description.cubeMapTarget->GetType());
    //     //     attachments.push_back(m_Description.cubeMapTarget);
    //     // }

    //     Graphics::RenderPassDesc renderPassDesc;
    //     renderPassDesc.attachmentCount = uint32_t(attachmentTypes.size());
    //     renderPassDesc.attachmentTypes = attachmentTypes.data();
    //     renderPassDesc.attachments = attachments.data();
    //     renderPassDesc.clear = m_Description.clearTargets;
    //     renderPassDesc.cubeMapIndex = m_Description.cubeMapIndex;
    //     renderPassDesc.mipIndex = m_Description.mipIndex;

    //     m_RenderPass = Graphics::RenderPass::Get(renderPassDesc);

    //     FramebufferDesc frameBufferDesc{};
    //     frameBufferDesc.width = GetWidth();
    //     frameBufferDesc.height = GetHeight();
    //     frameBufferDesc.attachmentCount = uint32_t(attachments.size());
    //     frameBufferDesc.renderPass = m_RenderPass.get();
    //     frameBufferDesc.attachmentTypes = attachmentTypes.data();
    //     frameBufferDesc.mipIndex = m_Description.mipIndex;
    //     if (m_Description.swapchainTarget)
    //     {
    //         for (uint32_t i = 0; i < Renderer::GetMainSwapChain()->GetSwapChainBufferCount(); i++)
    //         {
    //             frameBufferDesc.screenFBO = true;
    //             attachments[0] = Renderer::GetMainSwapChain()->GetImage(i);
    //             frameBufferDesc.attachments = attachments.data();

    //             m_Framebuffers.emplace_back(Framebuffer::Get(frameBufferDesc));
    //         }
    //     }
    //     else if (m_Description.depthArrayTarget)
    //     {
    //         // for (uint32_t i = 0; i < ((GLTextureDepthArray *)m_Description.depthArrayTarget)->GetCount(); ++i)
    //         // {
    //         //     frameBufferDesc.layer = i;
    //         //     frameBufferDesc.screenFBO = false;

    //         //     attachments[0] = m_Description.depthArrayTarget;
    //         //     frameBufferDesc.attachments = attachments.data();

    //         //     m_Framebuffers.emplace_back(Framebuffer::Get(frameBufferDesc));
    //         // }
    //     }
    //     else if (m_Description.cubeMapTarget)
    //     {
    //         // for (uint32_t i = 0; i < 6; ++i)
    //         //{
    //         //     frameBufferDesc.layer = i;
    //         //     frameBufferDesc.screenFBO = false;

    //         //    attachments[0] = m_Description.cubeMapTarget;
    //         //    frameBufferDesc.attachments = attachments.data();

    //         //    m_Framebuffers.emplace_back(Framebuffer::Get(frameBufferDesc));
    //         //}

    //         frameBufferDesc.layer = m_Description.cubeMapIndex;
    //         frameBufferDesc.attachments = attachments.data();
    //         frameBufferDesc.screenFBO = false;
    //         m_Framebuffers.emplace_back(Framebuffer::Get(frameBufferDesc));
    //     }
    //     else
    //     {
    //         frameBufferDesc.attachments = attachments.data();
    //         frameBufferDesc.screenFBO = false;
    //         m_Framebuffers.emplace_back(Framebuffer::Get(frameBufferDesc));
    //     }
}

// void VertexAtrribPointer(RHIFormat format, uint32_t index, size_t offset, uint32_t stride)
// {
//     switch (format)
//     {
//     case RHIFormat::R32_Float:
//         GLCall(glVertexAttribPointer(index, 1, GL_FLOAT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32_Float:
//         GLCall(glVertexAttribPointer(index, 2, GL_FLOAT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32_Float:
//         GLCall(glVertexAttribPointer(index, 3, GL_FLOAT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32A32_Float:
//         GLCall(glVertexAttribPointer(index, 4, GL_FLOAT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R8_UInt:
//         GLCall(glVertexAttribPointer(index, 1, GL_UNSIGNED_BYTE, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32_UInt:
//         GLCall(glVertexAttribPointer(index, 1, GL_UNSIGNED_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32_UInt:
//         GLCall(glVertexAttribPointer(index, 2, GL_UNSIGNED_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32_UInt:
//         GLCall(glVertexAttribPointer(index, 3, GL_UNSIGNED_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32A32_UInt:
//         GLCall(glVertexAttribPointer(index, 4, GL_UNSIGNED_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32_Int:
//         GLCall(glVertexAttribPointer(index, 2, GL_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32_Int:
//         GLCall(glVertexAttribPointer(index, 3, GL_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     case RHIFormat::R32G32B32A32_Int:
//         GLCall(glVertexAttribPointer(index, 4, GL_INT, false, stride, (const void *)(intptr_t)(offset)));
//         break;
//     }
// }

void GLPipeline::Bind(std::shared_ptr<CommandBuffer> commandBuffer, uint32_t layer)
{
    //     // GLRenderer::Instance()->GetBoundPipeline() = this;

    Framebuffer::Ptr framebuffer;

    if (mProperties->swapchainTarget)
    {
        // framebuffer = mFramebuffers[Renderer::GetMainSwapChain()->GetCurrentBufferIndex()];
    }
    else
    {
        framebuffer = mFramebuffers[0];
    }

    mRenderPass->BeginRenderPass(commandBuffer, mProperties->clearColor, framebuffer, Graphics::INLINE);
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
    case CullMode::BACK:
        glCullFace(GL_BACK);
        break;
    case CullMode::FRONT:
        glCullFace(GL_FRONT);
        break;
    case CullMode::FRONTANDBACK:
        glCullFace(GL_FRONT_AND_BACK);
        break;
    case CullMode::NONE:
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
        // GLRenderer::ClearInternal(RENDERER_BUFFER_COLOUR | RENDERER_BUFFER_DEPTH | RENDERER_BUFFER_STENCIL);
    }
}
