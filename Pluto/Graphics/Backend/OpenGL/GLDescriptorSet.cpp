/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto;
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
                unsigned int slot = descriptor.binding;
                unsigned int loc = glGetUniformBlockIndex(glshader->GetHandle(), descriptor.name.c_str());
                GlCall(glUniformBlockBinding(glshader->GetHandle(), loc, slot));
                auto bufferHandle = std::static_pointer_cast<GLUniformBuffer>(descriptor.ubo)->GetHandle();
                GlCall(glBindBufferRange(GL_UNIFORM_BUFFER, slot, bufferHandle, 0, descriptor.size));
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

void GLDescriptorSet::Update(SharedPtr<CommandBuffer> buffer)
{
    for (auto &bufferInfo : mUniformBuffers)
    {
        if (!bufferInfo.second.updated)
            continue;

        if (buffer == nullptr)
        {
            bufferInfo.second.ubo->SetData(bufferInfo.second.size, bufferInfo.second.data);
        }
        else
        {
            bufferInfo.second.ubo->SetData(bufferInfo.second.size, bufferInfo.second.data);
        }
        bufferInfo.second.updated = false;
    }
}

void GLDescriptorSet::SetTexture(const std::string &name, const SharedPtr<Texture> &texture, AttachmentType textureType, uint32_t mipIndex)
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
    if (mUniformBuffers.find(bufferName) != mUniformBuffers.end())
    {
        auto &itr = mUniformBuffers[bufferName];
        for (auto &member : itr.members)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr, data, member.size, member.offset);
                itr.updated = true;
                return;
            }
        }
    }
    log<Error>("GLDescriptorSet::SetUniform: Buffer %s-%s not found", bufferName.c_str(), uniformName.c_str());
}

void GLDescriptorSet::SetUniform(const std::string &bufferName, const std::string &uniformName, void *data, uint32_t size)
{
    if (mUniformBuffers.find(bufferName) != mUniformBuffers.end())
    {
        auto &itr = mUniformBuffers[bufferName];
        for (auto &member : itr.members)
        {
            if (member.name == uniformName)
            {
                this->WrtieUboInfoData(itr, data, size, member.offset);
                itr.updated = true;
                return;
            }
        }
    }
}

void GLDescriptorSet::SetUniformBufferData(const std::string &bufferName, void *data)
{
    if (mUniformBuffers.find(bufferName) != mUniformBuffers.end())
    {
        auto &itr = mUniformBuffers[bufferName];
        this->WrtieUboInfoData(itr, data, itr.size, 0);
        itr.updated = true;
        return;
    }
}

void GLDescriptorSet::SetBuffer(const std::string &name, const SharedPtr<UniformBuffer> &buffer)
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

SharedPtr<UniformBuffer> GLDescriptorSet::GetUniformBuffer(const std::string &name)
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

void GLDescriptorSet::Bind(uint32_t offset)
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
        // else
        // {
        //     auto ubo = std::static_pointer_cast<GLUniformBuffer>(descriptor.ubo);
        //     if (ubo == nullptr)
        //         break;
        //     uint32_t size = ubo->GetSize();
        //     if (ubo->GetDynamic())
        //     {
        //         size = ubo->GetDynamicSize();
        //     }

        //     auto bufferHandle = ubo->GetHandle();
        //     auto slot = descriptor.binding;
        //     GlCall(glBindBufferRange(GL_UNIFORM_BUFFER, slot, bufferHandle, offset, size));
        // }
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

void GLDescriptorSet::WrtieUboInfoData(UniformBufferInfo &info, void *data, uint32_t size, uint32_t offset)
{
    memcpy((uint8_t *)info.data + offset, data, size);
}