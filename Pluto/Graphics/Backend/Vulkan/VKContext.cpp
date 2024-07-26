/* Vulkan Context */
#include "Graphics/Backend/Vulkan/VKContext.h"
#include "Graphics/Backend/Vulkan/VKRenderContext.h"
/* Usage */
#include "Graphics/Backend/Vulkan/VKShader.h"
#include "Graphics/Backend/Vulkan/VKTexture.h"
#include "Graphics/Backend/Vulkan/VKPipeline.h"
#include "Graphics/Backend/Vulkan/VKSwapChain.h"
#include "Graphics/Backend/Vulkan/VKRenderPass.h"
#include "Graphics/Backend/Vulkan/VKIndexBuffer.h"
#include "Graphics/Backend/Vulkan/VKFramebuffer.h"
#include "Graphics/Backend/Vulkan/VKVertexBuffer.h"
#include "Graphics/Backend/Vulkan/VKCommandBuffer.h"
#include "Graphics/Backend/Vulkan/VKUniformBuffer.h"
#include "Graphics/Backend/Vulkan/VKDescriptorSet.h"
/* Common */

using namespace pluto;
using namespace pluto::Graphics;

namespace pluto::Graphics::Vulkan
{
    Shader::Ptr CreateShader(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKShader>(ctx, std::move((Shader::Properties *)pPropeties));
    }

    VertexBuffer::Ptr CreateVertexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKVertexBuffer>(ctx, std::move((VertexBuffer::Properties *)pPropeties));
    }

    IndexBuffer::Ptr CreateIndexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKIndexBuffer>(ctx, std::move((IndexBuffer::Properties *)pPropeties));
    }

    RenderPass::Ptr CreateRenderPass(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKRenderPass>(ctx, std::move((RenderPass::Properties *)pPropeties));
    }

    Framebuffer::Ptr CreateFrameBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKFramebuffer>(ctx, std::move((GLFramebuffer::Properties *)pPropeties));
    }

    CommandBuffer::Ptr CreateCommandBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKCommandBuffer>(ctx, std::move((CommandBuffer::Properties *)pPropeties));
    }

    UniformBuffer::Ptr CreateUniformBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKUniformBuffer>(ctx, std::move((UniformBuffer::Properties *)pPropeties));
    }

    SwapChain::Ptr CreateSwapChain(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKSwapChain>(ctx, std::move((SwapChain::Properties *)pPropeties));
    }

    Pipeline::Ptr CreatePipeline(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKPipeline>(ctx, std::move((Pipeline::Properties *)pPropeties));
    }

    DescriptorSet::Ptr CreateDescriptorSet(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<VKDescriptorSet>(ctx, std::move((DescriptorSet::Properties *)pPropeties));
    }

    Texture::Ptr CreateTexture(uint16_t type, RenderContext *ctx, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<VKTexture2D>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<VKTexture2DArray>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<VKTextureCube>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<VKTexture3D>(ctx, std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

    Texture::Ptr CreateTexture(uint16_t type, const std::string &path, RenderContext *ctx, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<VKTexture2D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<VKTexture2DArray>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<VKTextureCube>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<VKTexture3D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

}

VKContext::VKContext()
{
    mRenderContext = new VKRenderContext(this);
    mRenderAPI = RenderAPI::Vulkan;
}

VKContext::~VKContext() = default;

void VKContext::Present()
{
}

void VKContext::OnImGui()
{
}

void VKContext::Init()
{
    mRenderContext->Init();
}

SharedPtr<GraphicsContext> VKContext::Create()
{
    return std::make_shared<VKContext>();
}

Shader::Ptr VKContext::CreateShader(void *&&pPropeties)
{
    return Vulkan::CreateShader(mRenderContext, std::forward<void *>(pPropeties));
}

VertexBuffer::Ptr VKContext::CreateVertexBuffer(void *&&pPropeties)
{
    return Vulkan::CreateVertexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

IndexBuffer::Ptr VKContext::CreateIndexBuffer(void *&&pPropeties)
{
    return Vulkan::CreateIndexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

RenderPass::Ptr VKContext::CreateRenderPass(void *&&pPropeties)
{
    return Vulkan::CreateRenderPass(mRenderContext, std::forward<void *>(pPropeties));
}

Framebuffer::Ptr VKContext::CreateFrameBuffer(void *&&pPropeties)
{
    return Vulkan::CreateFrameBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<CommandBuffer> VKContext::CreateCommandBuffer(void *&&pPropeties)
{
    return Vulkan::CreateCommandBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<SwapChain> VKContext::CreateSwapChain(void *&&pPropeties)
{
    return Vulkan::CreateSwapChain(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<UniformBuffer> VKContext::CreateUniformBuffer(void *&&pPropeties)
{
    return Vulkan::CreateUniformBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<Pipeline> VKContext::CreatePipeline(void *&&pPropeties)
{
    return Vulkan::CreatePipeline(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<DescriptorSet> VKContext::CreateDescriptorSet(void *&&pPropeties)
{
    return Vulkan::CreateDescriptorSet(mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr VKContext::CreateTexture(uint16_t type, void *&&pPropeties)
{
    return Vulkan::CreateTexture(type, mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr VKContext::CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties)
{
    return Vulkan::CreateTexture(type, path, mRenderContext, std::forward<void *>(pPropeties));
}
