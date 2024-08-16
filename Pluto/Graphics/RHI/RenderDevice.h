#pragma once
#include "Core/Base.hpp"
#include "Graphics/RHI/RHIBase.h"
#include "Graphics/RHI/Declarations.h"

namespace pluto
{
    namespace Graphics
    {
        class RenderDevice
        {
        public:
            RenderDevice() = default;
            virtual ~RenderDevice() = default;

        public:
            static void Init();
            static RenderDevice *Create(const SharedPtr<GraphicsContext> &pContext);

        protected:
            inline static RenderDevice *(*CreateImpl)(const SharedPtr<GraphicsContext> &pContext){nullptr};
        };
    }
}
