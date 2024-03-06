#include "Engine/Render/Pass/AmbientPass.h"
#include "Engine/Render/Pass/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void AmbientPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/AmbientPass.frag");
            m_cubemap_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/CubeMap.vert",
                "Resource/Shader/Deferred/CubeMap.frag");
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          gtype::Format::RGBA, gtype::Format::RGBA16F, gtype::DataType::FLOAT);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            m_rt->Unbind();
            std::cout << "AmbientPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
        }
        void AmbientPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
        }
        void AmbientPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {

            Texture::ResetSlot();
            m_rt->Bind();
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            auto &camera = scene->GetCurrentCameraComponent();
            for (auto RTName : RenderStorageInputKey)
            {
                input.Get(RTName)->Activate();
                m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            }
            auto &root = scene->GetRootNode();
            auto cubemap = root->object->TryGetComponent<component::IBLCubeMap>();
            if (cubemap != nullptr && cubemap->valid)
            {
                auto &irridance = cubemap->IrridianceMap;
                auto &prefilter = cubemap->PrefilterMap;
                auto &brdflut = cubemap->BRDFLookupTexture;
                irridance->Activate();
                prefilter->Activate();
                brdflut->Activate();
                m_shader->SetUniform("irradianceMap", irridance->GetTempSlot());
                m_shader->SetUniform("prefilterMap", prefilter->GetTempSlot());
                m_shader->SetUniform("brdfLUT", brdflut->GetTempSlot());
                m_shader->SetUniform("hasIrradianceMap", 1.0f);
                m_shader->SetUniform("hasPrefilterMap", 1.0f);
                m_shader->SetUniform("hasBrdfLUT", 1.0f);
            }
            else
            {
                m_shader->SetUniform("hasIrradianceMap", 0.0f);
                m_shader->SetUniform("hasPrefilterMap", 0.0f);
                m_shader->SetUniform("hasBrdfLUT", 0.0f);
            }
            m_shader->SetUniform("ambientStrength", scene->GetConfig().ambient_strength);
            m_shader->SetUniform("viewPos", camera.GetPositionVector());
            EngineUtility::DrawQuad();
            if (cubemap != nullptr && cubemap->valid)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
                auto cube = MeshManager::Instance().GetMesh("Cube");
                cubemap->HDRCubeMap->Activate();
                input.Get("gStencil")->Activate();
                m_cubemap_shader->SetUniform("gStencil", input.Get("gStencil")->GetTempSlot());
                m_cubemap_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
                m_cubemap_shader->SetUniform("environmentMap", cubemap->HDRCubeMap->GetTempSlot());
                m_cubemap_shader->SetUniform("projection", camera.GetProjectionMatrix());
                m_cubemap_shader->SetUniform("view", camera.GetViewMatrix());
                cube->Draw();
                glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
                glDepthFunc(GL_LESS);
            }
            m_rt->Unbind();
            input.Insert(RenderStorageOutputKey[0], m_rt->GetTexture(0));
        }

    }
}