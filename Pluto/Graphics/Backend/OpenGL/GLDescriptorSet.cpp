/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLDescriptorSet::GLDescriptorSet(RenderContext *ctx, GLDescriptorSet::Properties *&&pProperties)
    : DescriptorSet(ctx, std::move(pProperties))
{
    auto glshader = std::static_pointer_cast<GLShader>(mProperties->shader);
    auto info = glshader->GetProperties().reflectInfo;
    mProperties->descriptorInfo.descriptors = info[mProperties->layoutIndex].descriptors;

    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::UniformBuffer)
        {
            auto buffer_desc = new UniformBuffer::Properties();
            buffer_desc->data = nullptr;
            buffer_desc->size = descriptor.size;
            descriptor.ubo = OpenGL::CreateUniformBuffer(mRenderContext, std::move(buffer_desc));

            UniformBufferInfo info;
            info.ubo = descriptor.ubo;
            info.updated = false;
            info.members = descriptor.mMembers;
            this->AllocateUboInfoData(info, descriptor.size);
            mUniformBuffers.emplace(descriptor.name, info);
            if (!descriptor.name.empty())
            {
                auto slot = descriptor.binding;
                auto loc = glGetUniformBlockIndex(glshader->GetHandle(), descriptor.name.c_str());
                GlCall(glUniformBlockBinding(glshader->GetHandle(), loc, slot));
            }
        }
    }
}

GLDescriptorSet::~GLDescriptorSet()
{
    for (auto &bufferInfo : mUniformBuffers)
    {
        this->ReleaseUboInfoData(bufferInfo.second);
    }
}

void GLDescriptorSet::Update(std::shared_ptr<CommandBuffer> buffer)
{
    for (auto &bufferInfo : mUniformBuffers)
    {
        if (bufferInfo.second.updated)
        {
            bufferInfo.second.ubo->SetData(bufferInfo.second.size, bufferInfo.second.data);
            bufferInfo.second.updated = false;
        }
    }
}

void GLDescriptorSet::SetTexture(const std::string &name, std::shared_ptr<Texture> texture, AttachmentType textureType, uint32_t mipIndex)
{
    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::ImageSampler && descriptor.name == name)
        {
            descriptor.texture = texture;
            descriptor.attachmentType = textureType;
            return;
        }
    }
}

void GLDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data)
{
    auto itr = mUniformBuffers.find(bufferName);
    if (itr != mUniformBuffers.end())
    {
        for (auto &member : itr->second.members)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr->second, data, member.size, member.offset);
                itr->second.updated = true;
                return;
            }
        }
    }
}

void GLDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size)
{
    auto itr = mUniformBuffers.find(bufferName);
    if (itr != mUniformBuffers.end())
    {
        for (auto &member : itr->second.members)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr->second, data, size, member.offset);
                itr->second.updated = true;
                return;
            }
        }
    }
}

void GLDescriptorSet::SetUniformBufferData(const std::string &bufferName, void *data)
{
    auto itr = mUniformBuffers.find(bufferName);
    if (itr != mUniformBuffers.end())
    {
        this->WrtieUboInfoData(itr->second, data, itr->second.size, 0);
        itr->second.updated = true;
        return;
    }
}

void GLDescriptorSet::SetBuffer(const std::string &name, std::shared_ptr<UniformBuffer> buffer)
{
    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::UniformBuffer && descriptor.name == name)
        {
            descriptor.ubo = buffer;
            return;
        }
    }
}

std::shared_ptr<UniformBuffer> GLDescriptorSet::GetUniformBuffer(const std::string &name)
{
    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::UniformBuffer && descriptor.name == name)
        {
            return descriptor.ubo;
        }
    }

    return nullptr;
}

void GLDescriptorSet::Bind(uint32_t offset = 0)
{
    auto glshader = std::static_pointer_cast<GLShader>(mProperties->shader);
    glshader->Bind();
    for (auto &descriptor : mProperties->descriptorInfo.descriptors)
    {
        if (descriptor.descType == DescriptorType::ImageSampler)
        {
            if (descriptor.texture != nullptr)
            {
                descriptor.texture->Bind(descriptor.binding);
                glshader->SetUniform(descriptor.name, descriptor.binding);
            }
        }
        else
        {
            auto ubo = std::static_pointer_cast<GLUniformBuffer>(descriptor.ubo);

            if (ubo == nullptr)
                break;

            uint8_t *data;
            uint32_t size;
            // Todo here
        }
    }
}

void GLDescriptorSet::AllocateUboInfoData(GLDescriptorSet::UniformBufferInfo &info, uint32_t size)
{
    if (info.data != nullptr)
        ReleaseUboInfoData(info);
    info.data = new uint8_t[size];
    info.size = size;
}

void GLDescriptorSet::ReleaseUboInfoData(GLDescriptorSet::UniformBufferInfo &info)
{
    if (info.data != nullptr)
        delete[] info.data;
}

void GLDescriptorSet::WrtieUboInfoData(UniformBufferInfo &info, void *data, uint32_t size, uint32_t offset = 0)
{
    memcpy((uint8_t *)info.data + offset, data, size);
}