#include "Engine/Render/Pass/SSRPass.h"
#include <random>
#include "Engine/Core/Utility.h"
namespace NoobRenderer
{
    namespace render
    {
        void SSRPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/SSRPass.vert",
                "Resource/Shader/Deferred/SSRPass.frag");
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height, Texture::Format::RGB, Texture::Format::RGB16F, Texture::DataType::FLOAT);
            m_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            std::cout << "SSRPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_rt->Unbind();
        }
        void SSRPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
        }
        void SSRPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            auto &camera = scene->GetCurrentCameraComponent();
            auto registry = scene->GetRegistry();
            auto view = registry->view<component::MeshData, component::ScreenSpaceReflection>();
            m_rt->Bind();
            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Texture::ResetSlot();
            for (auto RTName : RenderStorageInputKey)
            {
                input.Get(RTName)->Activate();
                m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            }
            m_shader->SetUniform("projection", camera.GetProjectionMatrix());
            m_shader->SetUniform("view", camera.GetViewMatrix());
            m_shader->SetUniform("invTView", glm::transpose(glm::inverse(camera.GetViewMatrix())));
            m_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            for (auto [entity, mesh_data, ssr] : view.each())
            {
                if (mesh_data.mesh == nullptr) continue;
                m_shader->SetUniform("model", scene->GetSceneNodeByEntity(entity)->model);
                m_shader->SetUniform("maxRayDistance", 100.f);
                mesh_data.mesh->Draw();
            }
            m_rt->Unbind();
            glDisable(GL_DEPTH_TEST);
            input.Insert("uSsrMap", m_rt->GetTexture());
        }
        unsigned int SSRPass::GetClampedDepth(unsigned int res)
        {
            // Todo: Hierachical Z-Buffer mipmap depth calculation
            int NumMipsX = std::max(static_cast<int>(std::ceil(std::log2(res) - 1.0)), 1);
            return static_cast<unsigned int>(1 << NumMipsX);
        }

    }
}