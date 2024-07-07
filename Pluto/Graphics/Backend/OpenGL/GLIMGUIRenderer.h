#pragma once

#include "Graphics/RHI/IMGUIRenderer.h"

namespace pluto
{
    namespace Graphics
    {
        class GLIMGUIRenderer : public IMGUIRenderer
        {
        public:
            GLIMGUIRenderer(uint32_t width, uint32_t height, bool clearScreen);
            ~GLIMGUIRenderer();

            void Init() override;
            void NewFrame() override;
            void Render(pluto::Graphics::CommandBuffer *commandBuffer) override;
            void OnResize(uint32_t width, uint32_t height) override;
            bool Implemented() const override { return true; }
            void RebuildFontTexture() override;
            // static void Init();

        protected:
            static IMGUIRenderer *CreateFuncGL(uint32_t width, uint32_t height, bool clearScreen);

        private:
            void *m_WindowHandle;
            bool m_ClearScreen;
        };
    }
}
