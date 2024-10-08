/* Vulkan Command Buffer */
#include "Graphics/Backend/Vulkan/VKRenderCommand.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"
#include "VKRenderCommand.h"

using namespace pluto;
using namespace pluto::Graphics;

VKRenderCommand::VKRenderCommand(RenderContext *ctx, VKRenderCommand::Properties *&&pProperties)
    : RenderCommand(ctx, std::move(pProperties))
{
    // Create Command Buffer
    switch (mProperties->type)
    {
    case CommandBufferUsageType::OneTimeSubmit:
    {
        mCommandBuffers.resize(1);
        auto pProperty = new CommandBuffer::Properties();
        pProperty->type = CommandBufferUsageType::OneTimeSubmit;
        mCommandBuffers[0] = std::static_pointer_cast<VKCommandBuffer>(Vulkan::CreateCommandBuffer(mRenderContext, std::move(pProperty)));
        mCommandBuffers[0]->Init(true);
        break;
    }
    case CommandBufferUsageType::RecycleSubmit:
    {
        auto pSwapChain = static_cast<VKSwapChain *>(mRenderContext->GetSwapChain());
        mCommandBuffers.resize(pSwapChain->GetSwapChainBufferCount());
        for (size_t i = 0; i < mCommandBuffers.size(); ++i)
        {
            auto pProperty = new CommandBuffer::Properties();
            pProperty->type = CommandBufferUsageType::RecycleSubmit;
            mCommandBuffers[i] = std::static_pointer_cast<VKCommandBuffer>(Vulkan::CreateCommandBuffer(mRenderContext, std::move(pProperty)));
            mCommandBuffers[i]->Init(true, pSwapChain->GetFrameCommandPool(i));
        }
        break;
    }
    default:
        break;
    }
}

VKRenderCommand::~VKRenderCommand()
{
    // CommandBuffers will be destroyed by VKRenderContext
}

void VKRenderCommand::Reset()
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        commandBuffer->Reset();
    }
}

bool VKRenderCommand::Flush()
{
    auto result = true;
    for (auto &commandBuffer : mCommandBuffers)
    {
        result &= commandBuffer->Flush();
    }
    return result;
}

void VKRenderCommand::BeginRecording()
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        commandBuffer->BeginRecording();
    }
}

void VKRenderCommand::EndRecording()
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        commandBuffer->EndRecording();
    }
}

void VKRenderCommand::BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding)
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        vbo->Bind(commandBuffer, pipeline, binding);
    }
}

void VKRenderCommand::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
    auto frameIndex = 0;
    auto pPipeline = std::static_pointer_cast<VKPipeline>(pipeline);
    for (auto &commandBuffer : mCommandBuffers)
    {
        uint32_t numDynamicDescriptorSets = 0;
        uint32_t numDescriptorSets = 0;
        VkDescriptorSet currentDescriptorSet;

        if (descriptorSet != nullptr)
        {
            auto vkDesSet = std::static_pointer_cast<VKDescriptorSet>(descriptorSet);
            if (vkDesSet->GetDynamic())
                numDynamicDescriptorSets++;

            currentDescriptorSet = vkDesSet->GetHandle(frameIndex);
            // NRE_ASSERT(vkDesSet->GetHasUpdated(frameIndex), "Descriptor Set has not been updated before");
            numDescriptorSets++;
        }
        else
        {
            PLog<PError>("Descriptor null");
        }

        vkCmdBindDescriptorSets(commandBuffer->GetHandle(), pPipeline->IsComputePipeline() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pPipeline->GetPipelineLayout(), 0, numDescriptorSets, &currentDescriptorSet, numDynamicDescriptorSets, &dynamicOffset);
    }
}

void VKRenderCommand::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
    auto frameIndex = 0;
    auto pPipeline = std::static_pointer_cast<VKPipeline>(pipeline);
    for (auto &commandBuffer : mCommandBuffers)
    {
        uint32_t numDynamicDescriptorSets = 0;
        uint32_t numDescriptorSets = 0;
        VkDescriptorSet lCurrentDescriptorSets[16] = {};

        for (auto &descSet : descriptorSets)
        {
            if (descSet != nullptr)
            {
                auto vkDesSet = std::static_pointer_cast<VKDescriptorSet>(descSet);
                if (vkDesSet->GetDynamic())
                    numDynamicDescriptorSets++;

                lCurrentDescriptorSets[numDescriptorSets] = vkDesSet->GetHandle(frameIndex);
                // NRE_ASSERT(vkDesSet->GetHasUpdated(frameIndex), "Descriptor Set has not been updated before");
                numDescriptorSets++;
            }
            else
            {
                PLog<PError>("Descriptor null");
            }
        }

        vkCmdBindDescriptorSets(commandBuffer->GetHandle(), pPipeline->IsComputePipeline() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pPipeline->GetPipelineLayout(), 0, numDescriptorSets, lCurrentDescriptorSets, numDynamicDescriptorSets, &dynamicOffset);
        frameIndex++;
    }
}

void VKRenderCommand::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
{
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    if (flipViewport)
    {
        viewport.width = (float)width;
        viewport.height = -(float)height;
        viewport.x = 0;
        viewport.y = (float)height;
    }

    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = {width, height};

    for (auto &commandBuffer : mCommandBuffers)
    {
        vkCmdSetViewport(commandBuffer->GetHandle(), 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer->GetHandle(), 0, 1, &scissor);
    }
}

void VKRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
    auto frameIndex = 0;
    //  mBoundPipelineLayer = 0;
    for (auto &commandBuffer : mCommandBuffers)
    {
        pipeline->Bind(commandBuffer, 0, frameIndex);
        ++frameIndex;
    }
    //   mBoundPipeline = pipeline;
}

void VKRenderCommand::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
    auto frameIndex = 0;
    // mBoundPipelineLayer = layer;
    for (auto &commandBuffer : mCommandBuffers)
    {
        pipeline->Bind(commandBuffer, layer, frameIndex);
        ++frameIndex;
    }
    mBoundPipeline = pipeline;
}

void VKRenderCommand::UnBindPipeline()
{
    if (mBoundPipeline == nullptr)
        return;

    for (auto &commandBuffer : mCommandBuffers)
    {
        mBoundPipeline->End(commandBuffer);
    }
}

void VKRenderCommand::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        vkCmdDrawIndexed(commandBuffer->GetHandle(), count, 1, start, 0, 0);
    }
}

void VKRenderCommand::Draw(DrawType type, uint32_t count)
{
    for (auto &commandBuffer : mCommandBuffers)
    {
        vkCmdDraw(commandBuffer->GetHandle(), count, 1, 0, 0);
    }
}

void VKRenderCommand::Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
    NRE_ASSERT(false, "Not Implemented Error");
}

void VKRenderCommand::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
    NRE_ASSERT(false, "Not Implemented Error");
}

SharedPtr<VKCommandBuffer> &pluto::Graphics::VKRenderCommand::GetCommandBuffer(uint32_t index)
{
    return mCommandBuffers[index];
}

void VKRenderCommand::Execute(uint32_t execIndex, VkPipelineStageFlags flags, VkSemaphore waitSemaphore, bool waitFence)
{
    mCommandBuffers[execIndex]->Execute(flags, waitSemaphore, waitFence);
}

void VKRenderCommand::Execute(uint32_t execIndex, VkPipelineStageFlags flags, VkSemaphore waitSemaphore, bool waitFence, VkSemaphore signalSemaphore)
{
    mCommandBuffers[execIndex]->Execute(flags, waitSemaphore, waitFence, signalSemaphore);
}