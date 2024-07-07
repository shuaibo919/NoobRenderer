#include "Graphics/Backend/OpenGL/GLRenderDevice.h"
using namespace pluto::Graphics;

GLRenderDevice::GLRenderDevice()
{
}
GLRenderDevice::~GLRenderDevice()
{
}

void GLRenderDevice::InitCreateImpl()
{
    RenderDevice::CreateImpl = CreateRenderDeviceGLImpl;
}

RenderDevice *GLRenderDevice::CreateRenderDeviceGLImpl()
{
    return new GLRenderDevice();
}