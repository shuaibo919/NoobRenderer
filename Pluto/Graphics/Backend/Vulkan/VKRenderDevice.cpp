#include "Graphics/Backend/Vulkan/VKRenderDevice.h"
using namespace pluto::Graphics;

VKRenderDevice::VKRenderDevice()
{
}

VKRenderDevice::~VKRenderDevice()
{
}

void VKRenderDevice::InitCreateImpl()
{
    RenderDevice::CreateImpl = CreateRenderDeviceVKImpl;
}

RenderDevice *VKRenderDevice::CreateRenderDeviceVKImpl()
{
    return new VKRenderDevice();
}