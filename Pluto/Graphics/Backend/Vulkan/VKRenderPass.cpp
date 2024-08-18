/* Vulkan RenderPass */
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VkSubpassContents GetSubPassContents(SubPassContents contents);
VkAttachmentDescription GetAttachmentDescription(AttachmentType type, const Texture::Ptr &texture, uint8_t samples = 1, bool clear = true);

VKRenderPass::VKRenderPass(RenderContext *ctx, VKRenderPass::Properties *&&pProperties)
    : RenderPass(ctx, std::move(pProperties))
{
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    std::vector<VkAttachmentDescription> attachments;

    std::vector<VkAttachmentReference> colourAttachmentReferences;
    std::vector<VkAttachmentReference> depthAttachmentReferences;
    std::vector<VkSubpassDependency> dependencies;

    mDepthOnly = true;
    mClearDepth = false;

    for (uint32_t i = 0; i < mProperties->attachments.size(); i++)
    {
        attachments.push_back(GetAttachmentDescription(mProperties->attachmentTypes[i], mProperties->attachments[i], mProperties->samples, mProperties->clear));

        if (mProperties->attachmentTypes[i] == AttachmentType::Color)
        {
            VkImageLayout layout = std::static_pointer_cast<VKTexture2D>(mProperties->attachments[i])->GetImageLayout();
            VkAttachmentReference colourAttachmentRef = {};
            colourAttachmentRef.attachment = uint32_t(i);
            colourAttachmentRef.layout = layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout;
            colourAttachmentReferences.push_back(colourAttachmentRef);
            mDepthOnly = false;
        }
        else if (mProperties->attachmentTypes[i] == AttachmentType::Depth)
        {
            /* TODO: */
            PLog<PCritical>("Not Implemented");
        }
        else if (mProperties->attachmentTypes[i] == AttachmentType::DepthArray)
        {
            /* TODO: */
            PLog<PCritical>("Not Implemented");
        }
        else if (mProperties->attachmentTypes[i] == AttachmentType::Cube)
        {
            /* TODO: */
            PLog<PCritical>("Not Implemented");
        }
        else
        {
            PLog<PError>("%s Not supported AttachmentType", PLineInfo);
        }

        if (mProperties->attachmentTypes[i] == AttachmentType::Depth || mProperties->attachmentTypes[i] == AttachmentType::DepthArray)
        {
            {
                VkSubpassDependency &depedency = dependencies.emplace_back();
                depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
                depedency.dstSubpass = 0;
                depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            }

            {
                VkSubpassDependency &depedency = dependencies.emplace_back();
                depedency.srcSubpass = 0;
                depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
                depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            }
        }
        else
        {
            {
                VkSubpassDependency &depedency = dependencies.emplace_back();
                depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
                depedency.dstSubpass = 0;
                depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            }
            {
                VkSubpassDependency &depedency = dependencies.emplace_back();
                depedency.srcSubpass = 0;
                depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
                depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
            }
        }
    }

    uint32_t attachmentCount = static_cast<uint32_t>(mProperties->attachments.size());

    bool resolveTexture = false;
    VkAttachmentReference colourAttachmentResolvedRef = {};

    if (mProperties->resolveTexture != nullptr && mProperties->samples > 1)
    {
        resolveTexture = true;
        VkImageLayout layout = std::static_pointer_cast<VKTexture2D>(mProperties->resolveTexture)->GetImageLayout();
        colourAttachmentResolvedRef.attachment = attachmentCount;
        colourAttachmentResolvedRef.layout = layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout;
        attachmentCount++;

        attachments.push_back(GetAttachmentDescription(mProperties->attachmentTypes[0], mProperties->resolveTexture, 1, mProperties->clear));
    }

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(colourAttachmentReferences.size());
    subpass.pColorAttachments = colourAttachmentReferences.data();
    subpass.pDepthStencilAttachment = depthAttachmentReferences.data();
    subpass.pResolveAttachments = resolveTexture ? &colourAttachmentResolvedRef : nullptr;

    mColorAttachments = static_cast<uint32_t>(colourAttachmentReferences.size());
    mDepthAttachments = static_cast<uint32_t>(depthAttachmentReferences.size());

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = uint32_t(attachmentCount);
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpass;
    renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassCreateInfo.pDependencies = dependencies.data();

    // mClearValue.resize(attachmentCount);
    mClearValue = new VkClearValue[attachmentCount];
    mClearValueCount = attachmentCount;
    VK_CHECK_RESULT(vkCreateRenderPass(pBasedDevice->GetDevice(), &renderPassCreateInfo, VK_NULL_HANDLE, &mRenderPass));
}

VKRenderPass::~VKRenderPass()
{
    RHIBase::Destroy();
}

void VKRenderPass::DestroyImplementation()
{
    if (mClearValue != nullptr)
        delete mClearValue;
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    vkDestroyRenderPass(pBasedDevice->GetDevice(), mRenderPass, VK_NULL_HANDLE);
}

void VKRenderPass::BeginRenderPass(const SharedPtr<CommandBuffer> &commandBuffer, float (&clearColor)[4], const SharedPtr<Framebuffer> &frame, SubPassContents contents) const
{
    if (!mDepthOnly)
    {
        for (uint32_t i = 0; i < mClearValueCount; ++i)
        {
            mClearValue[i].color.float32[0] = clearColor[0];
            mClearValue[i].color.float32[1] = clearColor[1];
            mClearValue[i].color.float32[2] = clearColor[2];
            mClearValue[i].color.float32[3] = clearColor[3];
        }
    }

    if (mClearDepth)
    {
        mClearValue[mClearValueCount - 1].depthStencil = VkClearDepthStencilValue{1.0f, 0};
    }

    VkRenderPassBeginInfo rpBegin = {};
    rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBegin.pNext = NULL;
    rpBegin.renderPass = mRenderPass;
    rpBegin.framebuffer = std::static_pointer_cast<VKFramebuffer>(frame)->GetHandle();
    rpBegin.renderArea.offset.x = 0;
    rpBegin.renderArea.offset.y = 0;
    rpBegin.renderArea.extent.width = frame->GetProperties().width;
    rpBegin.renderArea.extent.height = frame->GetProperties().height;
    rpBegin.clearValueCount = uint32_t(mClearValueCount);
    rpBegin.pClearValues = mClearValue;

    vkCmdBeginRenderPass(std::static_pointer_cast<VKCommandBuffer>(commandBuffer)->GetHandle(), &rpBegin, GetSubPassContents(contents));
    commandBuffer->UpdateViewport(frame->GetProperties().width, frame->GetProperties().height, mProperties->swapchainTarget);
}

void VKRenderPass::EndRenderPass(const SharedPtr<CommandBuffer> &commandBuffer)
{
    vkCmdEndRenderPass(std::static_pointer_cast<VKCommandBuffer>(commandBuffer)->GetHandle());
}

VkSubpassContents GetSubPassContents(SubPassContents contents)
{
    switch (contents)
    {
    case SubPassContents::Inline:
        return VK_SUBPASS_CONTENTS_INLINE;
    case SubPassContents::Secondary:
        return VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;
    default:
        return VK_SUBPASS_CONTENTS_INLINE;
    }
}

VkAttachmentDescription GetAttachmentDescription(AttachmentType type, const Texture::Ptr &texture, uint8_t samples, bool clear)
{
    VkAttachmentDescription attachment = {};
    if (type == AttachmentType::Color)
    {
        auto colourTexture = std::static_pointer_cast<VKTexture2D>(texture);
        attachment.format = colourTexture->GetFormat();
        attachment.initialLayout = colourTexture->GetImageLayout();
        attachment.finalLayout = attachment.initialLayout;
    }
    else if (type == AttachmentType::Cube)
    {
        auto colourTexture = std::static_pointer_cast<VKTextureCube>(texture);
        /* TODO: */
        PLog<PCritical>("Not Implemented");
    }
    else if (type == AttachmentType::Depth)
    {
        /* TODO: */
        PLog<PCritical>("Not Implemented");
    }
    else if (type == AttachmentType::DepthArray)
    {
        /* TODO: */
        PLog<PCritical>("Not Implemented");
    }
    else
    {
        PLog<PError>("%s Not supported AttachmentType", PLineInfo);
        return attachment;
    }

    if (clear)
    {
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }
    else
    {
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    }

    attachment.samples = samples > 1 ? (VkSampleCountFlagBits)samples : VK_SAMPLE_COUNT_1_BIT;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.flags = 0;

    return attachment;
}