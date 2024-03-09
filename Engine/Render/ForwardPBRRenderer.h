#pragma once
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Pass/ShadowPass.h"
#include "Engine/Render/Pass/ForwardPass.h"

namespace NoobRenderer
{
    namespace render
    {
        class ForwardPBRRenderer : public Renderer
        {
        public:
            const static int max_point_shadow_map = 4;
            using Ptr = std::shared_ptr<ForwardPBRRenderer>;
            ForwardPBRRenderer(unsigned int width, unsigned int height) : Renderer(width, height) {}
            void Resize(unsigned int width, unsigned int height) override;
            void Update() override;
            void Init() override;
            WriteToTexture::Ptr &GetRTHasGeometryInfo() override;

        public:
            ShadowPass ShadowMapPass;
            ForwardPass FPass;
        };
    }
}