#include "Graphics/RHI/GraphicsContext.h"

#ifdef OPENGL_BACKEND
#include "Graphics/Backend/OpenGL/GLContext.h"
#endif
#ifdef VULKAN_BACKEND
#include "Graphics/Backend/Vulkan/VKContext.h"
#endif

using namespace pluto::Graphics;

std::shared_ptr<GraphicsContext> GraphicsContext::Create(RenderAPI api, RenderDevice const *pDevice)
{
    switch (api)
    {
#ifdef VULKAN_BACKEND
    case RenderAPI::VULKAN:
        return Graphics::VulkanContext::Create(pDevice);
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

GraphicsContext::~GraphicsContext()
{
    if (mRenderContext != nullptr)
        delete mRenderContext;
}