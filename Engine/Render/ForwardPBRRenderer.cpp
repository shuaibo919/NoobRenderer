#include "Engine/Render/ForwardPBRRenderer.h"
#include <queue>
namespace NoobRenderer
{
    namespace render
    {

        void ForwardPBRRenderer::Init()
        {
            FPass.Init(RenderViewPort.width, RenderViewPort.height);
            ShadowMapPass.Init(4096, 4096, 0);
        }
        void ForwardPBRRenderer::Resize(unsigned int width, unsigned int height)
        {
            RenderViewPort.width = width;
            RenderViewPort.height = height;
            FPass.Rescale(width, height);
        }
        void ForwardPBRRenderer::Update()
        {
            auto current_scene = SceneManager::Instance().GetCurrentScene();
            if (current_scene == nullptr)
                return;
            current_scene->GetCurrentCameraComponent().Tick(RenderViewPort.width, RenderViewPort.height);
            ShadowMapPass.Render(current_scene, RTS);
            FPass.Render(current_scene, RTS);
            id = RTS.Get("Result")->GetID();
        }
        WriteToTexture::Ptr &ForwardPBRRenderer::GetGeometryInfo()
        {
            return FPass.GetRenderTexture();
        }

    }
}