/* OpenGL RenderPass */
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandCall.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;

GLRenderContext::GLRenderContext(GLContext *ctx)
    : mContext(ctx), RenderContext()
{
}

GLRenderContext::~GLRenderContext()
{
}

void GLRenderContext::Init()
{
    GlCall(glDisable(GL_DEPTH_TEST));
    GlCall(glDisable(GL_STENCIL_TEST));
    GlCall(glDisable(GL_CULL_FACE));
    GlCall(glDisable(GL_BLEND));
}

void GLRenderContext::Clear(uint32_t buffer)
{
    GlCall(glClear(GLUtilities::GetRendererBuffer(buffer)));
}

void GLRenderContext::Clear(const SharedPtr<CommandBuffer> &cmd, uint32_t buffer)
{
    OpenGL::EmulateCmdRecording(cmd, GlCmd(glClear(GLUtilities::GetRendererBuffer(buffer))));
}

void GLRenderContext::Begin()
{
    GlCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
    GlCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GLRenderContext::OnResize(uint32_t width, uint32_t height)
{
    std::static_pointer_cast<GLSwapChain>(mContext->mSwapChain)->OnResize(width, height);
}

void GLRenderContext::ClearRenderTarget(const SharedPtr<Texture> &texture, AttachmentType type, const SharedPtr<CommandBuffer> &commandBuffer, glm::vec4 clearColor)
{
    if (texture == nullptr)
    {
        GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        this->Clear(RenderBufferColor | RenderBufferDepth | RenderBufferStencil);
        return;
    }
    // Todo

    this->Clear(RenderBufferColor | RenderBufferDepth | RenderBufferStencil);
}

void GLRenderContext::Present()
{
}

void GLRenderContext::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void GLRenderContext::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
    for (auto &descriptorSet : descriptorSets)
    {
        if (descriptorSet != nullptr)
        {
            std::static_pointer_cast<GLDescriptorSet>(descriptorSet)->Bind(dynamicOffset);
        }
    }
}

void GLRenderContext::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
    if (descriptorSet != nullptr)
    {
        OpenGL::EmulateCmdRecording(commandBuffer, GlCmdWrap(std::static_pointer_cast<GLDescriptorSet>(descriptorSet)->Bind(dynamicOffset)));
    }
}

const std::string &GLRenderContext::GetTitle() const
{
    return mTitle;
}

void GLRenderContext::DrawIndexed(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count, uint32_t start) const
{
    if (!CurrentVertexHandle.valid || !CurrentIndiceHandle.valid)
        return;

    OpenGL::EmulateCmdRecording(cmd, GlCmd(glDrawElements(GLUtilities::GetDrawType(type), count, GLUtilities::GetDataType(DataType::UnsignedInt), nullptr)));
}

void GLRenderContext::Draw(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count) const
{
    if (!CurrentVertexHandle.valid)
        return;

    OpenGL::EmulateCmdRecording(cmd, GlCmd(glDrawArrays(GLUtilities::GetDrawType(type), 0, count)));
}

void GLRenderContext::Dispatch(const SharedPtr<CommandBuffer> &, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
}

void GLRenderContext::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
}

uint32_t GLRenderContext::GetGPUCount() const
{
    return 1;
}

bool GLRenderContext::SupportsCompute()
{
    return false;
}

RHIFormat GLRenderContext::GetDepthFormat()
{
    return RHIFormat::Depth32Float;
}

SwapChain::Ptr GLRenderContext::GetSwapChain()
{
    return mContext->mSwapChain;
}