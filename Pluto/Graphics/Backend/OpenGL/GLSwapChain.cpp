#include "Graphics/Backend/OpenGL/GLSwapChain.h"
#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
#include "Graphics/Backend/OpenGL/GLContext.h"
#include "Graphics/RHI/Framebuffer.h"

using namespace pluto::Graphics;
GLSwapChain::GLSwapChain(GLSwapChain::Properties *&&pProperties)
    : SwapChain(std::move(pProperties))
{
}

GLSwapChain::~GLSwapChain()
{
    for (auto &buffer : mBuffers)
        buffer.reset();
}
void GLSwapChain::OnResize(uint32_t width, uint32_t height)
{
}

bool GLSwapChain::Init(bool vsync)
{
    mCommand = std::dynamic_pointer_cast<GLCommandBuffer>(OpenGL::CreateCommandBuffer(new GLCommandBuffer::Properties()));
    return true;
}

Texture::Ptr GLSwapChain::GetCurrentImage()
{
    return nullptr; // swapChainBuffers[0];
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
    return nullptr;
}