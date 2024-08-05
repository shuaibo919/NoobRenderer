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

VKRenderContext::VKRenderContext(VKContext *ctx)
    : mContext(ctx), RenderContext()
{
}

VKRenderContext::~VKRenderContext()
{
}

void VKRenderContext::Init()
{
}

VKRenderDevice *VKRenderContext::GetBasedDevice() const
{
    return mContext->GetBasedDevice();
}

void VKRenderContext::Clear(uint32_t buffer)
{
}

void VKRenderContext::Begin()
{
}

void VKRenderContext::OnResize(uint32_t width, uint32_t height)
{
    std::static_pointer_cast<VKSwapChain>(mContext->mSwapChain)->OnResize(width, height);
}

void VKRenderContext::ClearRenderTarget(const SharedPtr<Texture> &texture, AttachmentType type, const SharedPtr<CommandBuffer> &commandBuffer, glm::vec4 clearColor)
{
}

void VKRenderContext::Present()
{
}

void VKRenderContext::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void VKRenderContext::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
}

void VKRenderContext::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, const SharedPtr<CommandBuffer> &commandBuffer, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
}

const std::string &VKRenderContext::GetTitle() const
{
    return mTitle;
}

void VKRenderContext::DrawIndexed(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count, uint32_t start) const
{
}

void VKRenderContext::Draw(const SharedPtr<CommandBuffer> &cmd, DrawType type, uint32_t count) const
{
}

void VKRenderContext::Dispatch(const SharedPtr<CommandBuffer> &, uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
}

void VKRenderContext::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
}

uint32_t VKRenderContext::GetGPUCount() const
{
    return 1;
}

bool VKRenderContext::SupportsCompute()
{
    return false;
}

RHIFormat VKRenderContext::GetDepthFormat()
{
    return RHIFormat::Depth32Float;
}

SwapChain::Ptr VKRenderContext::GetSwapChain()
{
    return mContext->mSwapChain;
}