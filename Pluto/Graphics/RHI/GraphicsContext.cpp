#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderContext.h"
#include "Graphics/RHI/SwapChain.h"
#ifdef OPENGL_BACKEND
#include "Graphics/Backend/OpenGL/GLContext.h"
#endif
#ifdef VULKAN_BACKEND
#include "Graphics/Backend/Vulkan/VKContext.h"
#endif

using namespace pluto::Graphics;

pluto::SharedPtr<GraphicsContext> GraphicsContext::Create(RenderAPI api)
{
    switch (api)
    {
#ifdef VULKAN_BACKEND
    case RenderAPI::VULKAN:
        return Graphics::VKContext::Create();
        break;
#endif

#ifdef OPENGL_BACKEND
    case RenderAPI::OPENGL:
        return Graphics::GLContext::Create();
        break;
#endif

    case RenderAPI::None:
        break;

    default:
        break;
    }
    return nullptr;
}

void GraphicsContext::SetMainSwapChain(const SharedPtr<SwapChain> &swapChain)
{
    mSwapChain = swapChain;
    mSwapChain->Init(false);
};

GraphicsContext::~GraphicsContext()
{
    if (mRenderContext != nullptr)
        delete mRenderContext;
}
