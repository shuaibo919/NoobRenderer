/* Vulkan VertexBuffer */
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"

using namespace pluto;
using namespace pluto::Graphics;
VKDescriptorSet::VKDescriptorSet(RenderContext *ctx, VKDescriptorSet::Properties *&&pProperties)
    : DescriptorSet(ctx, std::move(pProperties))
{
}

VKDescriptorSet::~VKDescriptorSet()
{
}

void VKDescriptorSet::Update(SharedPtr<CommandBuffer> buffer)
{
}

void VKDescriptorSet::SetTexture(const std::string &name, const SharedPtr<Texture> &texture, AttachmentType textureType, uint32_t mipIndex)
{
}

void VKDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data)
{
}

void VKDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size)
{
}

void VKDescriptorSet::SetUniformBufferData(const std::string &bufferName, void *data)
{
}

void VKDescriptorSet::SetBuffer(const std::string &name, const SharedPtr<UniformBuffer> &buffer)
{
}

SharedPtr<UniformBuffer> VKDescriptorSet::GetUniformBuffer(const std::string &name)
{
    return nullptr;
}

void VKDescriptorSet::Bind(uint32_t offset)
{
}