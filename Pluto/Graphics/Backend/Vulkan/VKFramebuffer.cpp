/* Vulkan Framebuffer */
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto::Graphics;
VKFramebuffer::VKFramebuffer(RenderContext *ctx, VKFramebuffer::Properties *&&pProperties)
    : Framebuffer(ctx, std::move(pProperties))
{

    std::vector<VkImageView> attachments;
    for (auto i = 0; i < mProperties->attachments.size(); i++)
    {
        switch (mProperties->attachmentTypes[i])
        {
        case AttachmentType::Color:
        {
            auto texture = std::static_pointer_cast<VKTexture2D>(mProperties->attachments[i]);
            attachments.push_back(mProperties->mipIndex >= 0 ? texture->GetMipImageView(mProperties->mipIndex) : texture->GetImageView());
            break;
        }
        case AttachmentType::Depth:
        {
            auto texture = std::static_pointer_cast<VKTexture2D>(mProperties->attachments[i]);
            attachments.push_back(texture->GetImageView());
            break;
        }
        case AttachmentType::Cube:
        {
            auto texture = std::static_pointer_cast<VKTextureCube>(mProperties->attachments[i]);
            attachments.push_back(mProperties->mipIndex >= 0 ? texture->GetImageView(mProperties->layer, mProperties->mipIndex) : texture->GetImageView(mProperties->layer));
            break;
        }
        case AttachmentType::DepthArray:
        {
            auto texture = std::static_pointer_cast<VKTexture2DArray>(mProperties->attachments[i]);
            attachments.push_back(texture->GetImageView(mProperties->layer));
            break;
            break;
        }
        case AttachmentType::Other:
        {
            auto texture = std::static_pointer_cast<VKTexture2D>(mProperties->attachments[i]);
            attachments.push_back(texture->GetImageView());
            break;
        }
        default:
            break;
        }
    }

    VkFramebufferCreateInfo framebufferCreateInfo = {};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = std::static_pointer_cast<VKRenderPass>(mProperties->renderPass)->GetHandle();
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    framebufferCreateInfo.pAttachments = attachments.data();
    framebufferCreateInfo.width = mProperties->width;
    framebufferCreateInfo.height = mProperties->height;
    framebufferCreateInfo.layers = 1;
    framebufferCreateInfo.pNext = nullptr;
    framebufferCreateInfo.flags = 0;

    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();

    VK_CHECK_RESULT(vkCreateFramebuffer(pBasedDevice->GetDevice(), &framebufferCreateInfo, VK_NULL_HANDLE, &mFramebuffer));
}

VKFramebuffer::~VKFramebuffer()
{
    auto pContext = static_cast<VKRenderContext *>(mRenderContext);
    auto device = pContext->GetBasedDevice()->GetDevice();
    auto framebuffer = mFramebuffer;

    pContext->PushDestoryTask([device, framebuffer]()
                              { vkDestroyFramebuffer(device, framebuffer, VK_NULL_HANDLE); });
}

void VKFramebuffer::SetClearColor(const glm::vec4 &color)
{
}