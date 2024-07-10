#include "Graphics/RHI/RenderDevice.h"
using namespace pluto::Graphics;

#ifdef OPENGL_BACKEND
#include "Graphics/Backend/OpenGL/GLRenderDevice.h"
#endif
#ifdef VULKAN_BACKEND
#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
#endif

void RenderDevice::Init()
{
#ifdef OPENGL_BACKEND
    GLRenderDevice::InitCreateImpl();
#endif
#ifdef VULKAN_BACKEND
    VKRenderDevice::InitCreateImpl();
#endif
}

void RenderDevice::Create()
{
    sInstance = CreateImpl();
}

void RenderDevice::Release()
{
    delete sInstance;
};