/* Vulkan Pipeline */
#include "Graphics/Backend/Vulkan/VKPipeline.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;
VKPipeline::VKPipeline(RenderContext *ctx, VKPipeline::Properties *&&pProperties)
    : Pipeline(ctx, std::move(pProperties))
{
    auto pBasedDevice = static_cast<VKRenderContext *>(mRenderContext)->GetBasedDevice();
    mShader = std::static_pointer_cast<VKShader>(mProperties->shader);

    this->TransitionAttachments();

    // Pipeline
    std::vector<VkDynamicState> dynamicStateDescriptors;
    VkPipelineDynamicStateCreateInfo dynamicStateCI{};
    dynamicStateCI.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCI.pNext = NULL;
    dynamicStateCI.pDynamicStates = dynamicStateDescriptors.data();

    std::vector<VkVertexInputAttributeDescription> vertexInputDescription;

    if (mShader->HasComputeStage())
    {
        VkComputePipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.layout = mShader->GetPipelineLayout();
        pipelineInfo.stage = mShader->GetShaderStages()[0];
        VK_CHECK_RESULT(vkCreateComputePipelines(pBasedDevice->GetDevice(), nullptr, 1, &pipelineInfo, nullptr, &mPipeline));
    }
    // TODO:
}

VKPipeline::~VKPipeline()
{
}

void VKPipeline::TransitionAttachments()
{
}

void VKPipeline::PrepareFramebuffer()
{
}

void VKPipeline::Bind(const SharedPtr<CommandBuffer> &commandBuffer, uint32_t layer)
{
}

void VKPipeline::End(const SharedPtr<CommandBuffer> &commandBuffer)
{
}

void VKPipeline::ClearRenderTargets(const SharedPtr<CommandBuffer> &commandBuffer)
{
}
