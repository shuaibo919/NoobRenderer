#include "Engine/Render/Pass/ShadowPass.h"
#include "Engine/Render/Pass/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void ShadowPass::Init(unsigned int main_shadow_ressolution, unsigned int point_shadow_resolution, unsigned int binding)
        {
            m_cascade_shadow_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Shadow/CascadeShadow.vert",
                "Resource/Shader/Shadow/CascadeShadow.frag",
                "Resource/Shader/Shadow/CascadeShadow.geom");

            m_point_shadow_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Shadow/PointShadow.vert",
                "Resource/Shader/Shadow/PointShadow.frag",
                "Resource/Shader/Shadow/PointShadow.geom");
            m_main_shadow = std::make_shared<CascadeShadow>(main_shadow_ressolution, main_shadow_ressolution, 5, binding);
            for (int i = 0; i < max_point_shadow_map; ++i)
            {
                m_point_shadows[i] = std::make_shared<OmnidirectionalShadow>(point_shadow_resolution, point_shadow_resolution);
            }
        }
        void ShadowPass::Init(unsigned int main_shadow_ressolution, unsigned int point_shadow_resolution)
        {
            this->Init(main_shadow_ressolution, point_shadow_resolution, 0);
        }
        void ShadowPass::Rescale(unsigned int width, unsigned int height)
        {
        }
        void ShadowPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            glEnable(GL_DEPTH_TEST);
            auto registry = scene->GetRegistry();
            auto view = registry->view<component::PointLight>();
            auto camera = scene->GetCurrentCameraComponent();
            auto viewFrustum = EngineUtility::GetFrustumCornersWorldSpace(camera.GetProjectionMatrix(), camera.GetViewMatrix());

            for (auto [entity, pointlight] : view.each())
            {
                if (pointlight.shadow && pointlight.shadow_channel < max_point_shadow_map && IsShadowRangeInViewFrustum(pointlight, viewFrustum))
                {
                    m_point_shadow_shader->SetUniform("lightPosition", pointlight.position);
                    m_point_shadow_shader->SetUniform("farPlane", pointlight.far);
                    auto matrices = m_point_shadows[pointlight.shadow_channel]->GetLightSpace(pointlight);
                    for (int i = 0; i < matrices.size(); ++i)
                    {
                        m_point_shadow_shader->SetUniform("shadowMatrices[" + std::to_string(i) + "]", matrices[i]);
                    }
                    m_point_shadows[pointlight.shadow_channel]->WrtieToMap(m_point_shadow_shader, scene, pointlight);
                }
            }
            auto dir_view = registry->view<component::DirectionalLight>();
            auto far = scene->GetCurrentCameraComponent().GetFar();
            for (auto [entity, dirlight] : dir_view.each())
            {
                if (dirlight.shadow)
                {
                    m_main_shadow->WrtieToMap(m_cascade_shadow_shader, scene, dirlight);
                    dirlight.cascade_levels = m_main_shadow->GetCascadeLevels(dirlight.cascade_splits, far);
                }
                break;
            }
            input.Insert(RenderStorageOutputKey[0], m_main_shadow->GetTexture(), 1);
            input.Insert(RenderStorageOutputKey[1], m_point_shadows[0]->GetTexture(), 1);
            input.Insert(RenderStorageOutputKey[2], m_point_shadows[1]->GetTexture(), 1);
        }

        bool ShadowPass::IsShadowRangeInViewFrustum(component::PointLight light, const std::vector<glm::vec4> &viewFrustum)
        {
            for (const auto &corner : viewFrustum)
            {
                float distance = glm::distance(light.position, glm::vec3(corner));
                if (distance <= light.far)
                {
                    return true;
                }
            }
            return false;
        }
    }
}