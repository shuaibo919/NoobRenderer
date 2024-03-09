#pragma once
#include "Engine/Render/Renderer.h"
#include "Engine/Render/Pass/GeometryPass.h"
#include "Engine/Render/Pass/LightPass.h"
#include "Engine/Render/Pass/IndirectLightPass.h"
#include "Engine/Render/Pass/AmbientPass.h"
#include "Engine/Render/Pass/ShadowPass.h"
#include "Engine/Render/Pass/SSAOPass.h"
#include "Engine/Render/Pass/FinalPass.h"
#include "Engine/Render/Pass/SSRPass.h"

namespace NoobRenderer
{
    namespace render
    {
        class DeferredRenderer : public Renderer
        {
        public:
            using Ptr = std::shared_ptr<DeferredRenderer>;
            DeferredRenderer(unsigned int width, unsigned int height) : Renderer(width, height) {}
            void Resize(unsigned int width, unsigned int height) override;
            void Update() override;
            void Init() override;
            WriteToTexture::Ptr &GetGeometryInfo() override;

        public:
            GeometryPass GPass;
            ShadowPass ShadowMapPass;
            LightPass LightPass;
            AmbientPass AmPass;
            IndirectLightPass IndirectLightPass;
            FinalPass FinalPass;
            SSAOPass SsaoPass;
            SSRPass SsrPass;
        };
    }
}