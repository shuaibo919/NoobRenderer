/* OpenGL SwapChain */
#include "Graphics/Backend/OpenGL/GLSwapChain.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/Backend/OpenGL/GLRenderDevice.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLSwapChain::GLSwapChain(RenderContext *ctx, GLSwapChain::Properties *&&pProperties)
    : SwapChain(ctx, std::move(pProperties))
{
    // GL has no swapchain, so we just create a empty implementation
}

GLSwapChain::~GLSwapChain()
{
}
void GLSwapChain::OnResize(uint32_t width, uint32_t height)
{
    mProperties->width = width;
    mProperties->height = height;
}

bool GLSwapChain::Init(bool vsync)
{
    mCommand = std::dynamic_pointer_cast<GLCommandBuffer>(
        OpenGL::CreateCommandBuffer(this->mRenderContext, new GLCommandBuffer::Properties()));
    return true;
}

Texture::Ptr GLSwapChain::GetCurrentImage()
{
    return nullptr;
}

pluto::SharedPtr<Texture> GLSwapChain::GetImage(uint32_t index)
{
    return nullptr;
};

uint32_t GLSwapChain::GetCurrentImageIndex() const
{
    return 0;
}

void GLSwapChain::SetVSync(bool vsync)
{
}

uint32_t GLSwapChain::GetCurrentBufferIndex() const
{
    return 0;
}

size_t GLSwapChain::GetSwapChainBufferCount() const
{
    return 1;
}
CommandBuffer::Ptr GLSwapChain::GetCurrentCommandBuffer()
{
    return mCommand;
}