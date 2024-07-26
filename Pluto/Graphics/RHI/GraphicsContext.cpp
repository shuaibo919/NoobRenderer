#include "Graphics/RHI/GraphicsContext.h"
#include "Graphics/RHI/RenderContext.h"
#ifdef OPENGL_BACKEND
#include "Graphics/Backend/OpenGL/GLContext.h"
#endif
#ifdef VULKAN_BACKEND
#include "Graphics/Backend/Vulkan/VKContext.h"
#endif

using namespace pluto::Graphics;

pluto::SharedPtr<GraphicsContext> GraphicsContext::Create(RenderAPI api, RenderDevice const *pDevice)
{
    switch (api)
    {
#ifdef VULKAN_BACKEND
    case RenderAPI::VULKAN:
        return Graphics::VKContext::Create(pDevice);
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
