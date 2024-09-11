#pragma once

#include "Graphics/RHI/IMGUIRenderer.h"

namespace pluto
{
    namespace Graphics
    {
        class VKImGuiRenderer : public ImGuiRenderer
        {
        public:
            VKImGuiRenderer(uint32_t width, uint32_t height, bool clearScreen);
            ~VKImGuiRenderer();

            void Init() override;
            void NewFrame() override;
            void Render(pluto::Graphics::CommandBuffer *commandBuffer) override;
            void OnResize(uint32_t width, uint32_t height) override;
            bool Implemented() const override { return true; }
            void RebuildFontTexture() override;

        protected:
            static ImGuiRenderer *CreateFuncVK(uint32_t width, uint32_t height, bool clearScreen);
        };
    }
}
