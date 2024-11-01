/* OpenGL Context */
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLRenderContext.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLIndexBuffer.h"
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderDevice.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderCommand.h"
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"
#include "Graphics/Backend/OpenGL/GLDescriptorSet.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"

using namespace pluto;
using namespace pluto::Graphics;

namespace pluto::Graphics::OpenGL
{
    Shader::Ptr CreateShader(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLShader>(ctx, std::move((Shader::Properties *)pPropeties));
    }

    VertexBuffer::Ptr CreateVertexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLVertexBuffer>(ctx, std::move((VertexBuffer::Properties *)pPropeties));
    }

    IndexBuffer::Ptr CreateIndexBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLIndexBuffer>(ctx, std::move((IndexBuffer::Properties *)pPropeties));
    }

    RenderPass::Ptr CreateRenderPass(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLRenderPass>(ctx, std::move((RenderPass::Properties *)pPropeties));
    }

    Framebuffer::Ptr CreateFrameBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLFramebuffer>(ctx, std::move((GLFramebuffer::Properties *)pPropeties));
    }

    CommandBuffer::Ptr CreateCommandBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLCommandBuffer>(ctx, std::move((CommandBuffer::Properties *)pPropeties));
    }

    RenderCommand::Ptr CreateRenderCommand(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLRenderCommand>(ctx, std::move((RenderCommand::Properties *)pPropeties));
    }

    UniformBuffer::Ptr CreateUniformBuffer(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLUniformBuffer>(ctx, std::move((UniformBuffer::Properties *)pPropeties));
    }

    SwapChain::Ptr CreateSwapChain(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLSwapChain>(ctx, std::move((SwapChain::Properties *)pPropeties));
    }

    Pipeline::Ptr CreatePipeline(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLPipeline>(ctx, std::move((Pipeline::Properties *)pPropeties));
    }

    DescriptorSet::Ptr CreateDescriptorSet(RenderContext *ctx, void *&&pPropeties)
    {
        return std::make_shared<GLDescriptorSet>(ctx, std::move((DescriptorSet::Properties *)pPropeties));
    }

    Texture::Ptr CreateTexture(uint16_t type, RenderContext *ctx, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<GLTexture2D>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<GLTexture2DArray>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<GLTextureCube>(ctx, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<GLTexture3D>(ctx, std::move((Texture::Properties *)pPropeties));
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
            return std::make_shared<GLTexture2D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<GLTexture2DArray>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<GLTextureCube>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<GLTexture3D>(ctx, path, std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

}

GLContext::GLContext()
{
    mRenderContext = new GLRenderContext(this);
    mRenderAPI = RenderAPI::OPENGL;
}

void GLContext::SetMainSwapChain(SwapChain::Properties &&properties)
{
    GLSwapChain *pSwapChain = new GLSwapChain(mRenderContext, new SwapChain::Properties(std::forward<SwapChain::Properties>(properties)));
    pSwapChain->Init(false);
    static_cast<GLRenderContext *>(mRenderContext)->SetSwapchain(pSwapChain);
}

GLContext::~GLContext()
{
    NRE_ASSERT(mTerminated, "Context is not terminated");
}

void GLContext::Terminate()
{
    delete mDevice;
    delete mRenderContext;
    mTerminated = true;
}

void GLContext::Present()
{
}

void GLContext::OnImGui()
{
}

void GLContext::BindToDevice()
{
    mDevice = GLRenderDevice::Create(this->Get());
}

void GLContext::Init()
{
    mRenderContext->Init();
}

SharedPtr<GraphicsContext> GLContext::Create()
{
    return std::make_shared<GLContext>();
}

int GLContext::LoadGladProc(void *proc)
{
    return gladLoadGLLoader((GLADloadproc)(proc));
}

Shader::Ptr GLContext::CreateShader(void *&&pPropeties)
{
    return OpenGL::CreateShader(mRenderContext, std::forward<void *>(pPropeties));
}

VertexBuffer::Ptr GLContext::CreateVertexBuffer(void *&&pPropeties)
{
    return OpenGL::CreateVertexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

IndexBuffer::Ptr GLContext::CreateIndexBuffer(void *&&pPropeties)
{
    return OpenGL::CreateIndexBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

RenderPass::Ptr GLContext::CreateRenderPass(void *&&pPropeties)
{
    return OpenGL::CreateRenderPass(mRenderContext, std::forward<void *>(pPropeties));
}

Framebuffer::Ptr GLContext::CreateFrameBuffer(void *&&pPropeties)
{
    return OpenGL::CreateFrameBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<CommandBuffer> GLContext::CreateCommandBuffer(void *&&pPropeties)
{
    return OpenGL::CreateCommandBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<RenderCommand> GLContext::CreateRenderCommand(void *&&pPropeties)
{
    return OpenGL::CreateRenderCommand(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<SwapChain> GLContext::CreateSwapChain(void *&&pPropeties)
{
    return OpenGL::CreateSwapChain(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<UniformBuffer> GLContext::CreateUniformBuffer(void *&&pPropeties)
{
    return OpenGL::CreateUniformBuffer(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<Pipeline> GLContext::CreatePipeline(void *&&pPropeties)
{
    return OpenGL::CreatePipeline(mRenderContext, std::forward<void *>(pPropeties));
}

SharedPtr<DescriptorSet> GLContext::CreateDescriptorSet(void *&&pPropeties)
{
    return OpenGL::CreateDescriptorSet(mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr GLContext::CreateTexture(uint16_t type, void *&&pPropeties)
{
    return OpenGL::CreateTexture(type, mRenderContext, std::forward<void *>(pPropeties));
}

Texture::Ptr GLContext::CreateTexture(uint16_t type, const std::string &path, void *&&pPropeties)
{
    return OpenGL::CreateTexture(type, path, mRenderContext, std::forward<void *>(pPropeties));
}
