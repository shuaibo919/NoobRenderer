#pragma once
#include "Engine/Render/Pass/RenderPass.h"
#include "Engine/Render/Collections/CascadeShadow.h"
#include "Engine/Render/Collections/OmnidirectionalShadow.h"
namespace NoobRenderer
{
    namespace render
    {
        class ShadowPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_cascade_shadow_shader{nullptr};
            ShaderProgram::Ptr m_point_shadow_shader{nullptr};
            // View内最多支持两张点光源阴影贴图
            const static int max_point_shadow_map = 2;
            CascadeShadow::Ptr m_main_shadow{nullptr};
            OmnidirectionalShadow::Ptr m_point_shadows[max_point_shadow_map] = {nullptr, nullptr};
            bool IsShadowRangeInViewFrustum(component::PointLight light, const std::vector<glm::vec4> &viewFrustum);

            std::array<std::string, 3> RenderStorageOutputKey{
                "mainDirLight.shadowMap",
                "pointLightShadowMap[0]",
                "pointLightShadowMap[1]",
            };

        public:
            using Ptr = std::shared_ptr<ShadowPass>;
            void Init(unsigned int main_shadow_ressolution = 4096, unsigned int point_shadow_resolution = 4096) override;
            void Init(unsigned int main_shadow_ressolution, unsigned int point_shadow_resolution, unsigned int binding);
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
