#pragma once
#include "Core/Base.hpp"

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
            static void Create();
            static void Release();
            static RenderDevice const *Get() { return sInstance; }

        protected:
            inline static RenderDevice *(*CreateImpl)(){nullptr};

        private:
            inline static RenderDevice *sInstance{nullptr};
        };
    }
}
