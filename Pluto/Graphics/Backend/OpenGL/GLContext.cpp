#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLVertexBuffer.h"
#include "Graphics/Backend/OpenGL/GLIndexBuffer.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLRenderPass.h"
#include "Graphics/Backend/OpenGL/GLPipeline.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GLSwapChain.h"

using namespace pluto::Graphics;

namespace pluto::Graphics::OpenGL
{
    VertexBuffer::Ptr CreateVertexBuffer(void *&&pPropeties)
    {
        return std::make_shared<GLVertexBuffer>(std::move((VertexBuffer::Properties *)pPropeties));
    }

    IndexBuffer::Ptr CreateIndexBuffer(void *&&pPropeties)
    {
        return std::make_shared<GLIndexBuffer>(std::move((IndexBuffer::Properties *)pPropeties));
    }

    RenderPass::Ptr CreateRenderPass(void *&&pPropeties)
    {
        return std::make_shared<GLRenderPass>(std::move((RenderPass::Properties *)pPropeties));
    }

    Framebuffer::Ptr CreateFrameBuffer(void *&&pPropeties)
    {
        return std::make_shared<GLFramebuffer>(std::move((GLFramebuffer::Properties *)pPropeties));
    }

    CommandBuffer::Ptr CreateCommandBuffer(void *&&pPropeties)
    {
        return std::make_shared<GLCommandBuffer>(std::move((CommandBuffer::Properties *)pPropeties));
    }

    SwapChain::Ptr CreateSwapChain(void *&&pPropeties)
    {
        return std::make_shared<GLSwapChain>(std::move((SwapChain::Properties *)pPropeties));
    }
    Pipeline::Ptr CreatePipeline(void *&&pPropeties)
    {
        return std::make_shared<GLPipeline>(std::move((Pipeline::Properties *)pPropeties));
    }
    Texture::Ptr CreateTexture(uint16_t type, void *&&pPropeties)
    {
        Texture::Type kind = static_cast<Texture::Type>(type);
        switch (kind)
        {
        case Texture::Type::Texture2D:
            return std::make_shared<GLTexture2D>(std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture2DArray:
            return std::make_shared<GLTexture2DArray>(std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::TextureCube:
            return std::make_shared<GLTextureCube>(std::move((Texture::Properties *)pPropeties));
            break;
        case Texture::Type::Texture3D:
            return std::make_shared<GLTexture3D>(std::move((Texture::Properties *)pPropeties));
            break;

        default:
            break;
        }
        return nullptr;
    }

    Texture::Ptr CreateTexture(uint16_t type, const std::string &path, void *&&pPropetie)
    {
        return nullptr;
    }

}

GLContext::GLContext()
{
    mRenderAPI = RenderAPI::OPENGL;
}

GLContext::~GLContext() = default;

void GLContext::Present()
{
}

void GLContext::OnImGui()
{
}

void GLContext::Init()
{
}

std::shared_ptr<GraphicsContext> GLContext::Create()
{
    return std::make_shared<GLContext>();
}

int GLContext::LoadGladProc(void *proc)
{
    return gladLoadGLLoader((GLADloadproc)(proc));
}

VertexBuffer::Ptr GLContext::CreateVertexBuffer(void *&&pPropeties)
{
    return OpenGL::CreateVertexBuffer(std::forward<void *>(pPropeties));
}

IndexBuffer::Ptr GLContext::CreateIndexBuffer(void *&&pPropeties)
{
    return OpenGL::CreateIndexBuffer(std::forward<void *>(pPropeties));
}

RenderPass::Ptr GLContext::CreateRenderPass(void *&&pPropeties)
{
    return OpenGL::CreateRenderPass(std::forward<void *>(pPropeties));
}

Framebuffer::Ptr GLContext::CreateFrameBuffer(void *&&pPropeties)
{
    return OpenGL::CreateFrameBuffer(std::forward<void *>(pPropeties));
}

std::shared_ptr<CommandBuffer> GLContext::CreateCommandBuffer(void *&&pPropeties)
{
    return OpenGL::CreateCommandBuffer(std::forward<void *>(pPropeties));
}

std::shared_ptr<SwapChain> GLContext::CreateSwapChain(void *&&pPropeties)
{
    return OpenGL::CreateSwapChain(std::forward<void *>(pPropeties));
}

std::shared_ptr<Pipeline> GLContext::CreatePipeline(void *&&pPropeties)
{
    return OpenGL::CreatePipeline(std::forward<void *>(pPropeties));
}

Texture::Ptr GLContext::CreateTexture(uint16_t type, void *&&pPropeties)
{
    return OpenGL::CreateTexture(type, std::forward<void *>(pPropeties));
}

Texture::Ptr GLContext::CreateTexture(uint16_t type, const std::string &path, void *&&pPropetie)
{
    return OpenGL::CreateTexture(type, path, std::forward<void *>(pPropetie));
}
