/* Vulkan Command Buffer */
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;

VkCommandBufferUsageFlags GetCommandBufferUsageFlags(CommandBufferUsageType usage)
{
    switch (usage)
    {
    case CommandBufferUsageType::OneTimeSubmit:
        return VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    case CommandBufferUsageType::RecycleSubmit:
        return VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    default:
        break;
    }
    return VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
}

VKCommandBuffer::VKCommandBuffer(RenderContext *ctx, CommandBuffer::Properties *&&pProperties)
    : CommandBuffer(ctx, std::move(pProperties))
{
    this->Init();
}

VKCommandBuffer::~VKCommandBuffer()
{
    this->Unload();
}

void VKCommandBuffer::Submit()
{
    this->Execute(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, nullptr, false);
}

bool VKCommandBuffer::Wait()
{
    mFence->WaitAndReset();
    mProperties->state = CommandBufferState::Idle;

    return true;
}

void VKCommandBuffer::Reset()
{
    mProperties->state = CommandBufferState::Idle;
}

bool VKCommandBuffer::Flush()
{
    if (mProperties->state == CommandBufferState::Idle)
        return true;

    mRenderContext->WaitIdle();

    if (mProperties->state == CommandBufferState::Submitted)
        return this->Wait();

    return true;
}

void VKCommandBuffer::Unload()
{
    mRenderContext->WaitIdle();

    if (mProperties->state == CommandBufferState::Submitted)
        Wait();

    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    mFence = nullptr;
    mSemaphore = nullptr;
    vkFreeCommandBuffers(pBasedDevice->GetDevice(), mCommandPool, 1, &mCommandBuffer);
}

bool VKCommandBuffer::Init(bool primary)
{
    auto pRenderCtx = static_cast<VKRenderContext *>(mRenderContext);
    mPrimary = primary;
    mCommandPool = pRenderCtx->GetBasedDevice()->GetCommandPool()->GetHandle();
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = mCommandPool;
    commandBufferAllocateInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(pRenderCtx->GetBasedDevice()->GetDevice(), &commandBufferAllocateInfo, &mCommandBuffer));
    mSemaphore = std::make_shared<VKSemaphore>(pRenderCtx->GetBasedDevice()->GetDevice(), false);
    mFence = std::make_shared<VKFence>(pRenderCtx->GetBasedDevice()->GetDevice(), false);
    mFence->Reset();

    return true;
}

bool VKCommandBuffer::Init(bool primary, VkCommandPool commandPool)
{
    auto pRenderCtx = static_cast<VKRenderContext *>(mRenderContext);
    mPrimary = primary;
    mCommandPool = commandPool;
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = mCommandPool;
    commandBufferAllocateInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = 1;
    VK_CHECK_RESULT(vkAllocateCommandBuffers(pRenderCtx->GetBasedDevice()->GetDevice(), &commandBufferAllocateInfo, &mCommandBuffer));

    mSemaphore = std::make_shared<VKSemaphore>(pRenderCtx->GetBasedDevice()->GetDevice(), false);
    mFence = std::make_shared<VKFence>(pRenderCtx->GetBasedDevice()->GetDevice(), true);
    return true;
}

void VKCommandBuffer::BeginRecording()
{
    if (mProperties->state != CommandBufferState::Recording) [[likely]]
        mProperties->state = CommandBufferState::Recording;
    else
    {
        PAssert(false, "CommandBuffer is already recording");
    }
    VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = GetCommandBufferUsageFlags(mProperties->type);
    VK_CHECK_RESULT(vkBeginCommandBuffer(mCommandBuffer, &cmdBufferBeginInfo));
}

void VKCommandBuffer::BeginRecordingSecondary(const SharedPtr<RenderPass> &renderPass, const SharedPtr<Framebuffer> &framebuffer)
{
    mProperties->state = CommandBufferState::Recording;

    VkCommandBufferInheritanceInfo cmdBufferInheritanceInfo = {};
    cmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufferInheritanceInfo.subpass = 0;
    cmdBufferInheritanceInfo.renderPass = std::static_pointer_cast<VKRenderPass>(renderPass)->GetHandle();
    cmdBufferInheritanceInfo.framebuffer = std::static_pointer_cast<VKFramebuffer>(framebuffer)->GetHandle();

    VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = &cmdBufferInheritanceInfo;

    VK_CHECK_RESULT(vkBeginCommandBuffer(mCommandBuffer, &cmdBufferBeginInfo));
}

void VKCommandBuffer::EndRecording()
{
    mProperties->state = CommandBufferState::Ended;
    vkEndCommandBuffer(mCommandBuffer);
}

void VKCommandBuffer::Execute(VkPipelineStageFlags flags, VkSemaphore signalSemaphore, bool waitFence)
{
    PAssert(mPrimary, "Used Execute on secondary command buffer!");
    PAssert(mProperties->state != CommandBufferState::Recording, "CommandBuffer executed before ended recording");
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    uint32_t waitSemaphoreCount = signalSemaphore != nullptr ? 1 : 0;

    VkSemaphore semaphore = mSemaphore->GetHandle();
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = VK_NULL_HANDLE;
    submitInfo.waitSemaphoreCount = waitSemaphoreCount;
    submitInfo.pWaitSemaphores = waitSemaphoreCount == 0 ? nullptr : &signalSemaphore;
    submitInfo.pWaitDstStageMask = &flags;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCommandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &semaphore;

    mFence->Reset();

    VK_CHECK_RESULT(vkQueueSubmit(pBasedDevice->GetGraphicsQueue(), 1, &submitInfo, mFence->GetHandle()));

    mProperties->state = CommandBufferState::Submitted;
}

void VKCommandBuffer::ExecuteSecondary(const SharedPtr<CommandBuffer> &primaryCmdBuffer)
{
    PAssert(false, "Not Implemented Error");
}

void VKCommandBuffer::Present(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void VKCommandBuffer::BindVetexBuffer(const SharedPtr<Pipeline> &pipeline, const SharedPtr<VertexBuffer> &vbo, uint8_t binding)
{
    vbo->Bind(this->GetSharedThis(), pipeline, binding);
}

void VKCommandBuffer::BindDescriptorSet(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, const SharedPtr<DescriptorSet> &descriptorSet)
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto pPipeline = std::static_pointer_cast<VKPipeline>(pipeline);
    uint32_t numDynamicDescriptorSets = 0;
    uint32_t numDescriptorSets = 0;
    VkDescriptorSet currentDescriptorSet;

    if (descriptorSet != nullptr)
    {
        auto vkDesSet = std::static_pointer_cast<VKDescriptorSet>(descriptorSet);
        if (vkDesSet->GetDynamic())
            numDynamicDescriptorSets++;

        uint32_t currentFrame = pRenderContext->GetSwapChain()->GetCurrentBufferIndex();
        currentDescriptorSet = vkDesSet->GetHandle(currentFrame);

        PAssert(vkDesSet->GetHasUpdated(currentFrame), "Descriptor Set has not been updated before");
        numDescriptorSets++;
    }
    else
    {
        PLog<PError>("Descriptor null");
    }

    vkCmdBindDescriptorSets(mCommandBuffer, pPipeline->IsComputePipeline() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pPipeline->GetPipelineLayout(), 0, numDescriptorSets, &currentDescriptorSet, numDynamicDescriptorSets, &dynamicOffset);
}

void VKCommandBuffer::BindDescriptorSets(const SharedPtr<Pipeline> &pipeline, uint32_t dynamicOffset, std::vector<SharedPtr<DescriptorSet>> &descriptorSets)
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto pPipeline = std::static_pointer_cast<VKPipeline>(pipeline);
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

            uint32_t currentFrame = pRenderContext->GetSwapChain()->GetCurrentBufferIndex();
            lCurrentDescriptorSets[numDescriptorSets] = vkDesSet->GetHandle(currentFrame);

            PAssert(vkDesSet->GetHasUpdated(currentFrame), "Descriptor Set has not been updated before");
            numDescriptorSets++;
        }
        else
        {
            PLog<PError>("Descriptor null");
        }
    }

    vkCmdBindDescriptorSets(mCommandBuffer, pPipeline->IsComputePipeline() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pPipeline->GetPipelineLayout(), 0, numDescriptorSets, lCurrentDescriptorSets, numDynamicDescriptorSets, &dynamicOffset);
}

void VKCommandBuffer::UpdateViewport(uint32_t width, uint32_t height, bool flipViewport)
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

    vkCmdSetViewport(mCommandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(mCommandBuffer, 0, 1, &scissor);
}

void VKCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline)
{
    if (pipeline != mBoundPipeline) [[likely]]
    {
        mBoundPipelineLayer = 0;

        if (mBoundPipeline)
            mBoundPipeline->End(this->GetSharedThis());

        pipeline->Bind(this->GetSharedThis());
        mBoundPipeline = pipeline;
    }
}

void VKCommandBuffer::BindPipeline(const SharedPtr<Pipeline> &pipeline, uint32_t layer)
{
    if (pipeline != mBoundPipeline || mBoundPipelineLayer != layer) [[likely]]
    {
        mBoundPipelineLayer = layer;

        if (mBoundPipeline)
            mBoundPipeline->End(this->GetSharedThis());

        pipeline->Bind(this->GetSharedThis(), layer);
        mBoundPipeline = pipeline;
    }
}

void VKCommandBuffer::UnBindPipeline()
{
    if (mBoundPipeline) [[likely]]
    {
        mBoundPipeline->End(this->GetSharedThis());
    }
    mBoundPipeline = nullptr;
}

void VKCommandBuffer::DrawIndexed(DrawType type, uint32_t count, uint32_t start)
{
    vkCmdDrawIndexed(mCommandBuffer, count, 1, start, 0, 0);
}

void VKCommandBuffer::Draw(DrawType type, uint32_t count)
{
    vkCmdDraw(mCommandBuffer, count, 1, 0, 0);
}

void VKCommandBuffer::Dispatch(uint32_t workGroupSizeX, uint32_t workGroupSizeY, uint32_t workGroupSizeZ)
{
    PAssert(false, "Not Implemented Error");
}

void VKCommandBuffer::DrawSplashScreen(const SharedPtr<Texture> &texture)
{
    PAssert(false, "Not Implemented Error");
}

VKCommandBuffer::Ptr VKCommandBuffer::GetSharedThis()
{
    return shared_from_this();
}