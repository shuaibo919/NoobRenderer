/* Vulkan RenderPass */
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
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

VkInstance VKRenderContext::GetVKInstance() const
{
    return mContext->GetVKInstance();
}

void VKRenderContext::Clear(uint32_t buffer)
{
}

void VKRenderContext::Begin()
{
}

void VKRenderContext::WaitIdle()
{
    vkDeviceWaitIdle(GetBasedDevice()->GetDevice());
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

void VKRenderContext::PushDestoryTask(std::function<void()> &&task)
{
    mDelayedDestoryTasks.push_back(std::forward<std::function<void()>>(task));
}

void VKRenderContext::ExecuteDestoryTasks()
{
    for (auto &deleteFunc : mDelayedDestoryTasks)
    {
        deleteFunc();
    }

    mDelayedDestoryTasks.clear();
}

VkDescriptorPool VKRenderContext::CreateDescriptorPool(uint32_t count, VkDescriptorPoolCreateFlags flags)
{
    std::array<VkDescriptorPoolSize, 11> poolSizes = {
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_SAMPLER, count / 2},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count * 4},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count * 2},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count * 2},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count},
        VkDescriptorPoolSize{VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count / 2}};
    VkDescriptorPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.flags = flags;
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.data();
    poolCreateInfo.maxSets = count;

    VkDescriptorPool descriptorPool;
    vkCreateDescriptorPool(this->GetBasedDevice()->GetDevice(), &poolCreateInfo, nullptr, &descriptorPool);

    return descriptorPool;
}

VkDescriptorPool VKRenderContext::GetDescriptorPool()
{

    if (mFreeDescriptorPools.size() > 0)
    {
        VkDescriptorPool pool = mFreeDescriptorPools.back();
        mFreeDescriptorPools.pop_back();
        return pool;
    }
    else
    {
        return CreateDescriptorPool(100, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
    }
}

bool VKRenderContext::AllocateDescriptorSet(VkDescriptorSet *set, VkDescriptorSetLayout layout, uint32_t descriptorCount)
{
    if (mCurrentPool == VK_NULL_HANDLE)
    {
        mCurrentPool = this->GetDescriptorPool();
        mUsedDescriptorPools.push_back(mCurrentPool);
    }

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.pSetLayouts = &layout;
    allocInfo.descriptorPool = mCurrentPool;
    allocInfo.descriptorSetCount = descriptorCount;

    VkResult allocResult = vkAllocateDescriptorSets(this->GetBasedDevice()->GetDevice(), &allocInfo, set);
    bool needReallocate = false;

    switch (allocResult)
    {
    case VK_SUCCESS:
        return true;

        break;
    case VK_ERROR_FRAGMENTED_POOL:
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        needReallocate = true;
        break;
    default:
        return false;
    }

    if (needReallocate)
    {
        // allocate a new pool and retry
        mCurrentPool = this->GetDescriptorPool();
        mUsedDescriptorPools.push_back(mCurrentPool);
        allocInfo.descriptorPool = mCurrentPool;

        allocResult = vkAllocateDescriptorSets(this->GetBasedDevice()->GetDevice(), &allocInfo, set);

        // if it still fails then we have big issues
        if (allocResult == VK_SUCCESS)
        {
            return true;
        }
    }

    return false;
}
