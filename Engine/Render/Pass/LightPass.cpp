#include "Engine/Render/Pass/LightPass.h"
#include "Engine/Render/Pass/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void LightPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/LightPass.frag");

            m_volumetric_light_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/VolumetricLightPass.frag");

            m_blur_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/ScreenBlur.frag");

            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          gtype::Format::RGBA, gtype::Format::RGBA32F, gtype::DataType::FLOAT);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            m_rt->Unbind();
            std::cout << "LightPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

            m_volumetric_light_rt = std::make_shared<WriteToTexture>(width, height);
            m_volumetric_light_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height);
            m_volumetric_light_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            m_volumetric_light_rt->Unbind();
            std::cout << "LightPass Volumetric RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

            m_blur_rt = std::make_shared<WriteToTexture>(width, height);
            m_blur_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height, gtype::Format::RGBA, gtype::Format::RGBA, gtype::DataType::FLOAT);
            m_blur_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            std::cout << "LightPass Blur Color RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_blur_rt->Unbind();
        }
        void LightPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
            m_volumetric_light_rt->Rescale(width, height);
            m_blur_rt->Rescale(width, height);
        }
        void LightPass::ProcessPointLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input)
        {
            auto point_lights = registry->view<component::PointLight>();
            auto light_count = 0;
            for (auto [entity, pointlight] : point_lights.each())
            {
                auto number = std::to_string(light_count++);
                m_shader->SetUniform("pointLights[" + number + "].position", pointlight.position);
                m_shader->SetUniform("pointLights[" + number + "].color", pointlight.color);
                m_shader->SetUniform("pointLights[" + number + "].constant", pointlight.constant);
                m_shader->SetUniform("pointLights[" + number + "].linear", pointlight.linear);
                m_shader->SetUniform("pointLights[" + number + "].quadratic", pointlight.quadratic);
                m_shader->SetUniform("pointLights[" + number + "].far", pointlight.far);
                m_shader->SetUniform("pointLights[" + number + "].castShadow", pointlight.shadow == true ? pointlight.shadow_channel : -1);
            }
            m_shader->SetUniform("nPointLights", light_count);
        }
        void LightPass::ProcessAreaLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input)
        {
        }
        void LightPass::ProcessMainLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input)
        {
            auto dir_lights = registry->view<component::DirectionalLight>();
            component::DirectionalLight *main_dir_light = nullptr;
            for (auto [entity, dirlight] : dir_lights.each())
            {
                main_dir_light = &dirlight;
                break;
            }

            m_shader->SetUniform("mainDirLight.enable", (main_dir_light == nullptr ? 0.f : 1.f));
            m_volumetric_light_shader->SetUniform("mainDirLight.enable", (main_dir_light == nullptr ? 0.f : 1.f));
            if (main_dir_light == nullptr)
            {
                m_shader->Use();
                return;
            }
            m_shader->SetUniform("mainDirLight.direction", main_dir_light->direction);
            m_shader->SetUniform("mainDirLight.color", main_dir_light->color);
            m_shader->SetUniform("mainDirLight.intensity", main_dir_light->intensity);
            m_shader->SetUniform("mainDirLight.castShadow", static_cast<float>(main_dir_light->shadow));
            m_shader->SetUniform("mainDirLight.cascadeCount", static_cast<int>(main_dir_light->cascade_levels.size()));
            m_shader->SetUniform("mainDirLight.biasFactor", main_dir_light->biasFactor);
            m_shader->SetUniform("mainDirLight.minBias", main_dir_light->minBias);
            m_shader->SetUniform("mainDirLight.shadowRadius", main_dir_light->shadowRadius);
            for (int i = 0; i < main_dir_light->cascade_levels.size(); i++)
            {
                auto number = std::to_string(i);
                m_shader->SetUniform("cascadePlaneDistances[" + number + "]", main_dir_light->cascade_levels[i]);
                m_shader->SetUniform("cascadeBiasModifier[" + number + "]", main_dir_light->biasModifier[i]);
            }
            m_volumetric_light_shader->SetUniform("mainDirLight.direction", main_dir_light->direction);
            m_volumetric_light_shader->SetUniform("mainDirLight.color", main_dir_light->color);
            m_volumetric_light_shader->SetUniform("mainDirLight.castShadow", static_cast<float>(main_dir_light->shadow));
            m_volumetric_light_shader->SetUniform("mainDirLight.cascadeCount", static_cast<int>(main_dir_light->cascade_levels.size()));
            m_volumetric_light_shader->SetUniform("mainDirLight.vStep", main_dir_light->vStep);
            m_volumetric_light_shader->SetUniform("mainDirLight.vScatter", main_dir_light->vScatter);
            m_volumetric_light_shader->SetUniform("mainDirLight.vIntensity", main_dir_light->vIntensity);
            for (int i = 0; i < main_dir_light->cascade_levels.size(); i++)
            {
                auto number = std::to_string(i);
                m_volumetric_light_shader->SetUniform("cascadePlaneDistances[" + number + "]", main_dir_light->cascade_levels[i]);
            }
            m_shader->Use();
        }
        void LightPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            /* Light  */
            Texture::ResetSlot();
            m_rt->Bind();
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            auto &camera = scene->GetCurrentCameraComponent();
            auto registry = scene->GetRegistry();
            auto dir_lights = registry->view<component::DirectionalLight>();
            m_shader->SetUniform("view", camera.GetViewMatrix());
            m_shader->SetUniform("viewPos", camera.GetPositionVector());
            m_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            for (auto RTName : RenderStorageInputKey)
            {
                input.Get(RTName)->Activate();
                m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            }
            ProcessPointLight(registry, input);
            ProcessMainLight(registry, input);
            EngineUtility::DrawQuad();
            m_rt->Unbind();
            input.Insert(RenderStorageOutputKey[0], m_rt->GetTexture(0));

            /* Blur */
            m_blur_rt->Bind();
            m_rt->ActivateTex();
            m_blur_shader->SetUniform("uScreenMap", m_rt->GetTexSlot());
            m_blur_shader->SetUniform("uResolution", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            m_blur_shader->SetUniform("uDirection", glm::vec2(1.0, 0.0f));
            EngineUtility::DrawQuad();
            m_blur_rt->Unbind();
            input.Insert(RenderStorageOutputKey[1], m_blur_rt->GetTexture(0));

            /* Volumetric Light */
            m_volumetric_light_rt->Bind();
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_volumetric_light_shader->SetUniform("view", camera.GetViewMatrix());
            m_volumetric_light_shader->SetUniform("viewPos", camera.GetPositionVector());
            m_volumetric_light_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            m_volumetric_light_shader->SetUniform("gPositionDepth", input.Get("gPositionDepth")->GetTempSlot());
            m_volumetric_light_shader->SetUniform("mainDirLight.shadowMap", input.Get("mainDirLight.shadowMap")->GetTempSlot());
            EngineUtility::DrawQuad();
            m_volumetric_light_rt->Unbind();
            input.Insert("uVolumetric", m_volumetric_light_rt->GetTexture(0));
        }
    }
}