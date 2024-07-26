/* Vulkan RenderPass */
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

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
}

void GLRenderContext::Clear(uint32_t buffer)
{
}

void GLRenderContext::Begin()
{
}

void GLRenderContext::OnResize(uint32_t width, uint32_t height)
{
    std::static_pointer_cast<GLSwapChain>(mContext->mSwapChain)->OnResize(width, height);
}

void GLRenderContext::ClearRenderTarget(const SharedPtr<Texture> &texture, AttachmentType type, const SharedPtr<CommandBuffer> &commandBuffer, glm::vec4 clearColor)
{
}

void GLRenderContext::Present()
{
}

void GLRenderContext::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void GLRenderContext::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
}

void GLRenderContext::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
}

const std::string &GLRenderContext::GetTitle() const
{
    return mTitle;
}

void GLRenderContext::DrawIndexed(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count, uint32_t start) const
{
    if (!CurrentVertexHandle.valid || !CurrentIndiceHandle.valid)
        return;

    glDrawElements(GLUtilities::GetDrawType(type), count, GLUtilities::GetDataType(DataType::UnsignedInt), nullptr);
}

void GLRenderContext::Draw(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count) const
{
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