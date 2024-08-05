/* Vulkan Command Buffer */
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKSync.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
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
    auto pRenderCtx = static_cast<VKRenderContext *>(mRenderContext);
    mPrimary = primary;
    mCommandPool = pRenderCtx->GetBasedDevice()->GetCommandPool()->GetHandle();
    VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = mCommandPool;
    commandBufferAllocateInfo.level = primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(pRenderCtx->GetBasedDevice()->GetDevice(), &commandBufferAllocateInfo, &mCommandBuffer));

    mSemaphore = std::make_shared<VKSemaphore>(false);
    mFence = std::make_shared<VKFence>(false);
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

    mSemaphore = std::make_shared<VKSemaphore>(false);
    mFence = std::make_shared<VKFence>(true);
    return true;
}

void VKCommandBuffer::Unload()
{
}

void VKCommandBuffer::BeginRecording()
{
    mProperties->state = CommandBufferState::Recording;
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
    // cmdBufferInheritanceInfo.renderPass = std::static_pointer_cast<VKRenderPass>(renderPass)->GetHandle();
    // cmdBufferInheritanceInfo.framebuffer = std::static_pointer_cast<VKFramebuffer>(framebuffer)->GetFramebuffer();

    VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    cmdBufferBeginInfo.pInheritanceInfo = &cmdBufferInheritanceInfo;

    VK_CHECK_RESULT(vkBeginCommandBuffer(mCommandBuffer, &cmdBufferBeginInfo));
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