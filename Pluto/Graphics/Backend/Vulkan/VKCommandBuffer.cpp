/* Vulkan Command Buffer */
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Usage */

/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;

VKCommandBuffer::VKCommandBuffer(RenderContext *ctx, Properties *&&pProperties)
    : CommandBuffer(ctx, std::move(pProperties))
{
}

VKCommandBuffer::~VKCommandBuffer()
{
}

void VKCommandBuffer::Submit()
{
  
}

bool VKCommandBuffer::Init(bool primary)
{
    return true;
}

void VKCommandBuffer::Unload()
{
}

void VKCommandBuffer::BeginRecording()
{
}

void VKCommandBuffer::BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer)
{
}

void VKCommandBuffer::EndRecording()
{
}

void VKCommandBuffer::ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer)
{
}

void VKCommandBuffer::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void VKCommandBuffer::BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding)
{
}

void VKCommandBuffer::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
}

void VKCommandBuffer::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
}

void VKCommandBuffer::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
{
}

void VKCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
}

void VKCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
}

void VKCommandBuffer::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
}

void VKCommandBuffer::Draw(DrawType type, uint32_t count)
{
}

void VKCommandBuffer::Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
}

void VKCommandBuffer::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
}

void VKCommandBuffer::UnBindPipeline()
{
}

void VKCommandBuffer::EndCurrentRenderPass()
{
}