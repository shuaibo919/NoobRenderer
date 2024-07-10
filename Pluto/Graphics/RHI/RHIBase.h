#pragma once
#include "Core/Base.hpp"

namespace pluto
{
    namespace Graphics
    {
        class RenderContext;
        class RHIBase
        {
        public:
            RHIBase(RenderContext *ctx) : mRenderContext(ctx){};
            virtual ~RHIBase() = default;

        protected:
            RenderContext *mRenderContext{nullptr};
        };
    }
}
