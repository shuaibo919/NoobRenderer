#include "Engine/Render/DeferredRenderer.h"
#include <queue>
namespace NoobRenderer
{
    namespace render
    {
        void DeferredRenderer::Init()
        {
            ShadowMapPass.Init(2048, 2048, 1);
            GPass.Init(RenderViewPort.width, RenderViewPort.height);
            LightPass.Init(RenderViewPort.width, RenderViewPort.height);
            FinalPass.Init(RenderViewPort.width, RenderViewPort.height);
            SsaoPass.Init(RenderViewPort.width, RenderViewPort.height);
            SsrPass.Init(RenderViewPort.width, RenderViewPort.height);
            IndirectLightPass.Init(RenderViewPort.width, RenderViewPort.height);
            AmPass.Init(RenderViewPort.width, RenderViewPort.height);
        }
        void DeferredRenderer::Resize(unsigned int width, unsigned int height)
        {
            RenderViewPort.width = width;
            RenderViewPort.height = height;
            GPass.Rescale(width, height);
            LightPass.Rescale(width, height);
            IndirectLightPass.Rescale(width, height);
            FinalPass.Rescale(width, height);
            SsaoPass.Rescale(width, height);
            SsrPass.Rescale(width, height);
            AmPass.Rescale(width, height);
        }
        void DeferredRenderer::Update()
        {
            auto current_scene = SceneManager::Instance().GetCurrentScene();
            if (current_scene == nullptr)
                return;
            current_scene->GetCurrentCameraComponent().Tick(RenderViewPort.width, RenderViewPort.height);
            GPass.Render(current_scene, RTS);
            SsaoPass.Render(current_scene, RTS);
            ShadowMapPass.Render(current_scene, RTS);
            LightPass.Render(current_scene, RTS);
            AmPass.Render(current_scene, RTS);
            SsrPass.Render(current_scene, RTS);
            IndirectLightPass.Render(current_scene, RTS);
            FinalPass.Render(current_scene, RTS);
            id = RTS.Get("Result")->GetID();
        }
        WriteToTexture::Ptr &DeferredRenderer::GetRTHasGeometryInfo()
        {
            return GPass.GetRenderTexture();
        }

    }
}