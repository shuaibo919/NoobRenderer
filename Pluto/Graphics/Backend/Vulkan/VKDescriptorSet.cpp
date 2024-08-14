/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKBuffer.h"
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;
VKDescriptorSet::VKDescriptorSet(RenderContext *ctx, VKDescriptorSet::Properties *&&pProperties)
    : DescriptorSet(ctx, std::move(pProperties))
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto vkshader = std::static_pointer_cast<VKShader>(mProperties->shader);
    auto info = vkshader->GetProperties().reflectInfo;
    mFlightFrameCount = uint32_t(pRenderContext->GetSwapChain()->GetSwapChainBufferCount());
    mProperties->descriptorInfo.descriptors = info[mProperties->layoutIndex].descriptors;

    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::UniformBuffer)
        {
            for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
            {
                // Uniform Buffer per frame in flight
                auto buffer_desc = new UniformBuffer::Properties();
                buffer_desc->data = nullptr;
                buffer_desc->size = descriptor.size;
                descriptor.ubo = Vulkan::CreateUniformBuffer(mRenderContext, std::move(buffer_desc));
                mUniformBuffers[frame][descriptor.name] = descriptor.ubo;
            }
            UniformBufferInfo info;
            info.HasUpdated[0] = false;
            info.HasUpdated[1] = false;
            info.HasUpdated[2] = false;
            info.mMembers = descriptor.mMembers;
            this->AllocateUboInfoData(info, descriptor.size);
            mUniformBuffersData[descriptor.name] = info;
        }
    }

    for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
    {
        mDescriptorDirty[frame] = true;
        mDescriptorUpdated[frame] = false;
        mDescriptorSet[frame] = VK_NULL_HANDLE;
        auto layout = std::static_pointer_cast<VKShader>(mProperties->shader)->GetDescriptorSetLayout(mProperties->layoutIndex);
        pRenderContext->AllocateDescriptorSet(&mDescriptorSet[frame], layout, 1);
    }
}

VKDescriptorSet::~VKDescriptorSet()
{
    for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
    {
        // Todo:
    }

    for (auto it = mUniformBuffersData.begin(); it != mUniformBuffersData.end(); it++)
    {
        this->ReleaseUboInfoData(it->second);
    }
}

void VKDescriptorSet::Update(SharedPtr<CommandBuffer> buffer)
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    int descriptorWritesCount = 0;
    uint32_t currentFrame = pRenderContext->GetSwapChain()->GetCurrentBufferIndex();

    for (auto &bufferInfo : mUniformBuffersData)
    {
        if (bufferInfo.second.HasUpdated[currentFrame])
        {
            mUniformBuffers[currentFrame][bufferInfo.first]->SetData(bufferInfo.second.size, bufferInfo.second.data);
            bufferInfo.second.HasUpdated[currentFrame] = false;
        }
    }

    if (mDescriptorDirty[currentFrame] || !mDescriptorUpdated[currentFrame])
    {
        mDescriptorDirty[currentFrame] = false;
        uint32_t imageIndex = 0;
        uint32_t index = 0;

        const uint32_t descriptorCount = static_cast<uint32_t>(mProperties->descriptorInfo.descriptors.size());

        std::array<VkDescriptorBufferInfo, 32> bufferInfos;
        std::array<VkDescriptorImageInfo, 32> imageInfos;
        std::array<VkWriteDescriptorSet, 32> writeDescriptorSets;

        for (auto &descInfo : mProperties->descriptorInfo.descriptors)
        {
            if (descInfo.descType == DescriptorType::ImageSampler && descInfo.texture != nullptr)
            {

                if (descInfo.texture)
                {
                    TransitionImageLayoutByHints(descInfo.texture, buffer);

                    VkDescriptorImageInfo &des = *static_cast<VkDescriptorImageInfo *>(descInfo.texture->GetDescriptorInfo());
                    imageInfos[imageIndex].imageLayout = des.imageLayout;
                    imageInfos[imageIndex].imageView = des.imageView;
                    imageInfos[imageIndex].sampler = des.sampler;
                }

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstSet = mDescriptorSet[currentFrame];
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                writeDescriptorSet.dstBinding = descInfo.binding;
                writeDescriptorSet.pImageInfo = &imageInfos[imageIndex];
                writeDescriptorSet.descriptorCount = static_cast<uint32_t>(mProperties->descriptorInfo.descriptors.size());

                writeDescriptorSets[descriptorWritesCount] = writeDescriptorSet;
                imageIndex++;
                descriptorWritesCount++;
            }
            else if (descInfo.descType == DescriptorType::ImageStorage && descInfo.texture != nullptr)
            {
                if (descInfo.texture)
                {
                    auto vktexure = std::static_pointer_cast<VKTexture2D>(descInfo.texture);
                    vktexure->TransitionImage(VK_IMAGE_LAYOUT_GENERAL, nullptr);

                    VkDescriptorImageInfo &des = *static_cast<VkDescriptorImageInfo *>(descInfo.texture->GetDescriptorInfo());
                    imageInfos[imageIndex].imageLayout = des.imageLayout;
                    imageInfos[imageIndex].imageView = descInfo.mipLevel > 0 ? vktexure->GetMipImageView(descInfo.mipLevel) : des.imageView;
                    imageInfos[imageIndex].sampler = des.sampler;
                }

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstSet = mDescriptorSet[currentFrame];
                writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                writeDescriptorSet.dstBinding = descInfo.binding;
                writeDescriptorSet.pImageInfo = &imageInfos[imageIndex];
                writeDescriptorSet.descriptorCount = static_cast<uint32_t>(mProperties->descriptorInfo.descriptors.size());

                writeDescriptorSets[descriptorWritesCount] = writeDescriptorSet;
                imageIndex++;
                descriptorWritesCount++;
            }

            else if (descInfo.descType == DescriptorType::UniformBuffer)
            {
                auto vkUniformBuffer = std::static_pointer_cast<VKUniformBuffer>(mUniformBuffers[currentFrame][descInfo.name]);
                bufferInfos[index].buffer = vkUniformBuffer->GetBuffer()->GetBuffer();
                bufferInfos[index].offset = descInfo.offset;
                bufferInfos[index].range = descInfo.size;

                VkWriteDescriptorSet writeDescriptorSet = {};
                writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                writeDescriptorSet.dstSet = mDescriptorSet[currentFrame];
                writeDescriptorSet.descriptorType = VKUtilities::GetDescriptorType(descInfo.descType);
                writeDescriptorSet.dstBinding = descInfo.binding;
                writeDescriptorSet.pBufferInfo = &bufferInfos[index];
                writeDescriptorSet.descriptorCount = 1;

                writeDescriptorSets[descriptorWritesCount] = writeDescriptorSet;
                index++;
                descriptorWritesCount++;

                if (descInfo.descType == DescriptorType::UniformBufferDynamic)
                {
                    mDynamic = true;
                }
            }
        }

        vkUpdateDescriptorSets(pRenderContext->GetBasedDevice()->GetDevice(), descriptorWritesCount,
                               writeDescriptorSets.data(), 0, nullptr);

        mDescriptorUpdated[currentFrame] = true;
    }
}

void VKDescriptorSet::SetTexture(const std::string &name, const SharedPtr<Texture> &texture, AttachmentType textureType, uint32_t mipIndex)
{
    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if ((descriptor.descType == DescriptorType::ImageSampler || descriptor.descType == DescriptorType::ImageStorage) && descriptor.name == name)
        {
            descriptor.texture = texture;
            descriptor.attachmentType = textureType;
            descriptor.mipLevel = mipIndex;
            for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
                mDescriptorDirty[frame] = true;
        }
    }
}

void VKDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data)
{
    std::map<std::string, UniformBufferInfo>::iterator itr = mUniformBuffersData.find(bufferName);
    if (itr != mUniformBuffersData.end())
    {
        for (auto &member : itr->second.mMembers)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr->second, data, member.size, member.offset);
                for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
                    itr->second.HasUpdated[frame] = true;
                return;
            }
        }
    }
}

void VKDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size)
{
    auto itr = mUniformBuffersData.find(bufferName);
    if (itr != mUniformBuffersData.end())
    {
        for (auto &member : itr->second.mMembers)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr->second, data, size, member.offset);
                for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
                    itr->second.HasUpdated[frame] = true;
                return;
            }
        }
    }
}

void VKDescriptorSet::SetUniformBufferData(const std::string &bufferName, void *data)
{
    auto itr = mUniformBuffersData.find(bufferName);
    if (itr != mUniformBuffersData.end())
    {
        this->WrtieUboInfoData(itr->second, data, itr->second.size, 0);
        for (uint32_t frame = 0; frame < mFlightFrameCount; frame++)
            itr->second.HasUpdated[frame] = true;
        return;
    }
}

void VKDescriptorSet::SetBuffer(const std::string &name, const SharedPtr<UniformBuffer> &buffer)
{
    PLog<PCritical>("SetBuffer is a Empty Implementation!");
}

SharedPtr<UniformBuffer> VKDescriptorSet::GetUniformBuffer(const std::string &name)
{
    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    uint32_t currentFrame = pRenderContext->GetSwapChain()->GetCurrentBufferIndex();

    PLog<PCritical>("GetUniformBuffer is a Empty Implementation!");
    return nullptr;
}

void VKDescriptorSet::AllocateUboInfoData(UniformBufferInfo &info, uint32_t size)
{
    if (info.data != nullptr)
        this->ReleaseUboInfoData(info);
    info.data = new uint8_t[size];
    info.size = size;
}

void VKDescriptorSet::ReleaseUboInfoData(UniformBufferInfo &info)
{
    if (info.data != nullptr)
        delete[] info.data;
}

void VKDescriptorSet::WrtieUboInfoData(UniformBufferInfo &info, void *data, uint32_t size, uint32_t offset)
{
    memcpy((uint8_t *)info.data + offset, data, size);
}

void VKDescriptorSet::TransitionImageLayoutByHints(const SharedPtr<Texture> &texture, const SharedPtr<CommandBuffer> &cmdBuffer)
{
    if (!texture)
        return;

    auto pRenderContext = static_cast<VKRenderContext *>(mRenderContext);
    auto commandBuffer = cmdBuffer ? cmdBuffer : pRenderContext->GetSwapChain()->GetCurrentCommandBuffer();
    auto commandBufferHandle = std::static_pointer_cast<VKCommandBuffer>(commandBuffer)->GetHandle();
    if (texture->GetProperties().hints == Texture::Hints::NoHints || texture->GetProperties().hints == Texture::Hints::ShaderUse)
    {
        auto vktexure = std::static_pointer_cast<VKTexture2D>(texture);
        if (vktexure->GetImageLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            vktexure->TransitionImage(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandBufferHandle);
        }
    }
    else if (texture->GetProperties().hints == Texture::Hints::CubeMap)
    {
    }
    else if (texture->GetProperties().hints == Texture::Hints::DepthMap)
    {
    }
    else if (texture->GetProperties().hints == Texture::Hints::ShadowMap)
    {
    }
}