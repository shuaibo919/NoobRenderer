#pragma once
#include "Graphics/RHI/RenderDevice.h"

namespace pluto
{
    namespace Graphics
    {
        class GLRenderDevice : public RenderDevice
        {
        public:
            GLRenderDevice();
            ~GLRenderDevice();

            static void InitCreateImpl();

        protected:
            static RenderDevice *CreateRenderDeviceGLImpl();
        };
    }
}
