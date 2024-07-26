#pragma once
#include "Graphics/RHI/RenderDevice.h"

namespace pluto
{
    namespace Graphics
    {
        class VKRenderDevice : public RenderDevice
        {
        public:
            VKRenderDevice();
            ~VKRenderDevice();

            static void InitCreateImpl();

        protected:
            static RenderDevice *CreateRenderDeviceVKImpl();
        };
    }
}
