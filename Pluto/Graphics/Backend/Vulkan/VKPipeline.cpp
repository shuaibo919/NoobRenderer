/* Vulkan Pipeline */
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;
VKPipeline::VKPipeline(RenderContext *ctx, VKPipeline::Properties *&&pProperties)
    : Pipeline(ctx, std::move(pProperties)), VKObjectManageByContext(static_cast<VKRenderContext *>(ctx))
{
    auto pBasedDevice = VKObjectManageByContext::Context->GetBasedDevice();
    mShader = std::static_pointer_cast<VKShader>(mProperties->shader);

    this->TransitionAttachments();

    // Pipeline
    std::vector<VkDynamicState> dynamicStateDescriptors;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateInfo.pNext = NULL;
    dynamicStateInfo.pDynamicStates = dynamicStateDescriptors.data();

    std::vector<VkVertexInputAttributeDescription> vertexInputDescription;

    if (mShader->HasComputeStage())
    {
        VkComputePipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = mShader->GetPipelineLayout();
        pipelineInfo.stage = mShader->GetShaderStages()[0];
        VK_CHECK_RESULT(vkCreateComputePipelines(pBasedDevice->GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &mPipeline));
    }
    else
    {
        this->PrepareFramebuffer();

        uint32_t stride = mShader->GetVertexInputStride();
        VkVertexInputBindingDescription vertexBindingDescription;

        if (stride > 0)
        {
            vertexBindingDescription.binding = 0;
            vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            vertexBindingDescription.stride = stride;
        }

        const std::vector<VkVertexInputAttributeDescription> &vertexInputAttributeDescription = mShader->GetVertexInputDescription();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = NULL;
        vertexInputInfo.vertexBindingDescriptionCount = stride > 0 ? 1 : 0;
        vertexInputInfo.pVertexBindingDescriptions = stride > 0 ? &vertexBindingDescription : nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = stride > 0 ? uint32_t(vertexInputAttributeDescription.size()) : 0;
        vertexInputInfo.pVertexAttributeDescriptions = stride > 0 ? vertexInputAttributeDescription.data() : nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyInfo.pNext = NULL;
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        inputAssemblyInfo.topology = VKUtilities::GetVKPrimitiveTopology(mProperties->drawType);

        VkPipelineRasterizationStateCreateInfo rasterStateInfo{};
        rasterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterStateInfo.polygonMode = VKUtilities::GetVKPolygonMode(mProperties->polygonMode);
        rasterStateInfo.cullMode = VKUtilities::GetVKCullMode(mProperties->cullMode);
        rasterStateInfo.frontFace = mProperties->swapchainTarget ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
        rasterStateInfo.depthClampEnable = VK_TRUE;
        rasterStateInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterStateInfo.depthBiasEnable = VK_FALSE;
        // Todo: Add Depth bias to properties
        rasterStateInfo.depthBiasConstantFactor = 0.0f;
        rasterStateInfo.depthBiasClamp = 0;
        rasterStateInfo.depthBiasSlopeFactor = 0.0f;
        rasterStateInfo.lineWidth = 1.0f;
        rasterStateInfo.pNext = NULL;

        VkPipelineColorBlendStateCreateInfo blendStateInfo{};
        blendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blendStateInfo.pNext = NULL;
        blendStateInfo.flags = 0;

        std::vector<VkPipelineColorBlendAttachmentState> blendAttachState;

        blendAttachState.resize(mRenderPass->GetColorAttachments());

        for (unsigned int i = 0; i < blendAttachState.size(); i++)
        {
            blendAttachState[i] = VkPipelineColorBlendAttachmentState();
            blendAttachState[i].colorWriteMask = 0x0f;
            blendAttachState[i].alphaBlendOp = VK_BLEND_OP_ADD;
            blendAttachState[i].colorBlendOp = VK_BLEND_OP_ADD;
            blendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            blendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;

            if (mProperties->transparencyEnabled)
            {
                blendAttachState[i].blendEnable = VK_TRUE;
                blendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                blendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;

                if (mProperties->blendMode == BlendMode::SrcAlphaOneMinusSrcAlpha)
                {
                    blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                    blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                    blendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                    blendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                }
                else if (mProperties->blendMode == BlendMode::SrcAlphaOne)
                {
                    blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                    blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
                    blendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                    blendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                }
                else if (mProperties->blendMode == BlendMode::ZeroSrcColor)
                {
                    blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                    blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
                }
                else if (mProperties->blendMode == BlendMode::OneZero)
                {
                    blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
                    blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                }
                else
                {
                    blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                    blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                }
            }
            else
            {
                blendAttachState[i].blendEnable = VK_FALSE;
                blendAttachState[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                blendAttachState[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                blendAttachState[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                blendAttachState[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            }
        }

        blendStateInfo.attachmentCount = static_cast<uint32_t>(blendAttachState.size());
        blendStateInfo.pAttachments = blendAttachState.data();
        blendStateInfo.logicOpEnable = VK_FALSE;
        blendStateInfo.logicOp = VK_LOGIC_OP_NO_OP;
        blendStateInfo.blendConstants[0] = 1.0f;
        blendStateInfo.blendConstants[1] = 1.0f;
        blendStateInfo.blendConstants[2] = 1.0f;
        blendStateInfo.blendConstants[3] = 1.0f;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.pNext = NULL;
        viewportInfo.viewportCount = 1;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = NULL;
        viewportInfo.pViewports = NULL;
        dynamicStateDescriptors.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        dynamicStateDescriptors.push_back(VK_DYNAMIC_STATE_SCISSOR);

        VkPipelineDepthStencilStateCreateInfo depthstencilStateInfo{};
        depthstencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthstencilStateInfo.pNext = NULL;
        depthstencilStateInfo.depthTestEnable = mProperties->DepthTest ? VK_TRUE : VK_FALSE;
        depthstencilStateInfo.depthWriteEnable = mProperties->DepthWrite ? VK_TRUE : VK_FALSE;
        depthstencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        depthstencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        depthstencilStateInfo.stencilTestEnable = VK_FALSE;
        depthstencilStateInfo.back.failOp = VK_STENCIL_OP_KEEP;
        depthstencilStateInfo.back.passOp = VK_STENCIL_OP_KEEP;
        depthstencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
        depthstencilStateInfo.back.compareMask = 0;
        depthstencilStateInfo.back.reference = 0;
        depthstencilStateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
        depthstencilStateInfo.back.writeMask = 0;
        depthstencilStateInfo.minDepthBounds = 0;
        depthstencilStateInfo.maxDepthBounds = 0;
        depthstencilStateInfo.front = depthstencilStateInfo.back;

        VkPipelineMultisampleStateCreateInfo multisampleInfo{};
        multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.pNext = NULL;
        multisampleInfo.pSampleMask = NULL;
        multisampleInfo.rasterizationSamples = mProperties->samples > 1 ? (VkSampleCountFlagBits)mProperties->samples : VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleInfo.alphaToOneEnable = VK_FALSE;
        multisampleInfo.minSampleShading = 0.0;

        dynamicStateInfo.dynamicStateCount = uint32_t(dynamicStateDescriptors.size());
        dynamicStateInfo.pDynamicStates = dynamicStateDescriptors.data();

        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
        graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineCreateInfo.pNext = NULL;
        graphicsPipelineCreateInfo.layout = mShader->GetPipelineLayout();
        graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
        graphicsPipelineCreateInfo.pVertexInputState = &vertexInputInfo;
        graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
        graphicsPipelineCreateInfo.pRasterizationState = &rasterStateInfo;
        graphicsPipelineCreateInfo.pColorBlendState = &blendStateInfo;
        graphicsPipelineCreateInfo.pTessellationState = VK_NULL_HANDLE;
        graphicsPipelineCreateInfo.pMultisampleState = &multisampleInfo;
        graphicsPipelineCreateInfo.pDynamicState = &dynamicStateInfo;
        graphicsPipelineCreateInfo.pViewportState = &viewportInfo;
        graphicsPipelineCreateInfo.pDepthStencilState = &depthstencilStateInfo;
        graphicsPipelineCreateInfo.pStages = mShader->GetShaderStages();
        graphicsPipelineCreateInfo.stageCount = mShader->GetStageCount();
        graphicsPipelineCreateInfo.renderPass = mRenderPass->GetHandle();
        graphicsPipelineCreateInfo.subpass = 0;

        VK_CHECK_RESULT(vkCreateGraphicsPipelines(pBasedDevice->GetDevice(), pBasedDevice->GetPipelineCache(), 1, &graphicsPipelineCreateInfo, VK_NULL_HANDLE, &mPipeline));
    }
}

VKPipeline::~VKPipeline()
{
    VKObjectManageByContext::Destroy();
}

void VKPipeline::DestroyImplementation()
{
    auto pBasedDevice = VKObjectManageByContext::Context->GetBasedDevice();
    vkDestroyPipeline(pBasedDevice->GetDevice(), mPipeline, VK_NULL_HANDLE);
}

void VKPipeline::TransitionAttachments()
{
    auto commandBuffer = std::static_pointer_cast<VKCommandBuffer>(VKObjectManageByContext::Context->GetSwapChain()->GetCurrentCommandBuffer());

    if (mProperties->swapchainTarget)
    {
        for (uint32_t i = 0; i < VKObjectManageByContext::Context->GetSwapChain()->GetSwapChainBufferCount(); i++)
        {
            std::static_pointer_cast<VKTexture2D>(VKObjectManageByContext::Context->GetSwapChain()->GetImage(i))->TransitionImage(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, commandBuffer->GetHandle());
        }
    }

    for (auto texture : mProperties->colorTargets)
    {
        if (texture != nullptr)
        {
            std::static_pointer_cast<VKTexture2D>(texture)->TransitionImage(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, commandBuffer->GetHandle());
        }
    }

    if (mProperties->resolveTexture)
    {
        std::static_pointer_cast<VKTexture2D>(mProperties->resolveTexture)->TransitionImage(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, commandBuffer->GetHandle());
    }

    // TODO:
}

void VKPipeline::PrepareFramebuffer()
{
    std::vector<AttachmentType> attachmentTypes;
    std::vector<Texture::Ptr> attachments;

    if (mProperties->swapchainTarget)
    {
        attachmentTypes.push_back(AttachmentType::Color);
        attachments.push_back(VKObjectManageByContext::Context->GetSwapChain()->GetImage(0));
    }
    else
    {
        for (int i = 0; i < mProperties->attachmentTypes.size(); i++)
        {
            if (mProperties->colorTargets[i] != nullptr)
            {
                attachmentTypes.push_back(mProperties->attachmentTypes[i]);
                attachments.push_back(mProperties->colorTargets[i]);
            }
        }
    }

    if (mProperties->resolveTexture != nullptr)
    {
        attachmentTypes.push_back(AttachmentType::Color);
        attachments.push_back(mProperties->resolveTexture);
    }

    auto renderPassProperties = new RenderPass::Properties();
    renderPassProperties->attachmentTypes = attachmentTypes;
    renderPassProperties->attachments = attachments;
    renderPassProperties->swapchainTarget = mProperties->swapchainTarget;
    renderPassProperties->clear = mProperties->clearTargets;
    renderPassProperties->mipIndex = mProperties->mipIndex;
    renderPassProperties->resolveTexture = mProperties->resolveTexture;
    renderPassProperties->samples = mProperties->samples;

    mRenderPass = std::static_pointer_cast<VKRenderPass>(Vulkan::CreateRenderPass(VKObjectManageByContext::Context, std::move(renderPassProperties)));
    renderPassProperties = nullptr;

    auto framebufferProperties = new Framebuffer::Properties();
    framebufferProperties->width = this->GetWidth();
    framebufferProperties->height = this->GetHeight();
    framebufferProperties->renderPass = mRenderPass;
    framebufferProperties->attachmentTypes = attachmentTypes;
    framebufferProperties->samples = mProperties->samples;

    if (mProperties->swapchainTarget)
    {
        for (uint32_t i = 0; i < VKObjectManageByContext::Context->GetSwapChain()->GetSwapChainBufferCount(); i++)
        {
            attachments[0] = VKObjectManageByContext::Context->GetSwapChain()->GetImage(i);
            framebufferProperties->screenUse = true;
            framebufferProperties->attachments = attachments;
            mFramebuffers.emplace_back(
                Vulkan::CreateFrameBuffer(VKObjectManageByContext::Context, new Framebuffer::Properties(*framebufferProperties)));
        }
    }
    else
    {
        framebufferProperties->attachments = attachments;
        framebufferProperties->screenUse = false;
        framebufferProperties->mipIndex = mProperties->mipIndex;
        mFramebuffers.emplace_back(Vulkan::CreateFrameBuffer(VKObjectManageByContext::Context, std::move(framebufferProperties)));
        framebufferProperties = nullptr;
    }
    delete framebufferProperties;
}

void VKPipeline::Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer, uint32_t frame)
{
    Framebuffer::Ptr framebuffer = mFramebuffers[0];
    if (!mShader->HasComputeStage())
    {
        this->TransitionAttachments();

        if (mProperties->swapchainTarget)
        {
            framebuffer = mFramebuffers[frame];
        }

        mRenderPass->BeginRenderPass(commandBuffer, mProperties->clearColor, framebuffer, Graphics::Inline);
    }
    else
    {
    }

    vkCmdBindPipeline(std::static_pointer_cast<VKCommandBuffer>(commandBuffer)->GetHandle(), mShader->HasComputeStage() ? VK_PIPELINE_BIND_POINT_COMPUTE : VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
}

void VKPipeline::End(const SharedPtr<CommandBuffer> &commandBuffer)
{
    if (!mShader->HasComputeStage())
    {
        mRenderPass->EndRenderPass(commandBuffer);
    }
}

void VKPipeline::ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer)
{
    if (mProperties->swapchainTarget)
    {
        for (uint32_t i = 0; i < VKObjectManageByContext::Context->GetSwapChain()->GetSwapChainBufferCount(); i++)
        {
            // TODO: ClearRenderTarget
        }
    }

    {
        for (auto texture : mProperties->colorTargets)
        {
            if (texture != nullptr)
            {
                // TODO: ClearRenderTarget
            }
        }
    }
}

bool VKPipeline::IsComputePipeline() const
{
    return mShader->HasComputeStage();
}

VkPipeline VKPipeline::GetHandle() const
{
    return mPipeline;
}

VkPipelineLayout VKPipeline::GetPipelineLayout() const
{
    return mShader->GetPipelineLayout();
}