#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class LightPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader{nullptr};
            ShaderProgram::Ptr m_volumetric_light_shader{nullptr};
            ShaderProgram::Ptr m_blur_shader{nullptr};
            WriteToTexture::Ptr m_volumetric_light_rt{nullptr};
            WriteToTexture::Ptr m_blur_rt{nullptr};
            std::array<std::string, 8> RenderStorageInputKey{
                "gPositionDepth",
                "gNormal",
                "gAlbedo",
                "gMetalRoughAo",
                "gEmission",
                "mainDirLight.shadowMap",
                "pointLightShadowMap[0]",
                "pointLightShadowMap[1]",
            };
            std::array<std::string, 2> RenderStorageOutputKey{
                "uColorMap",
                "uColorBlurMap",
            };
            void ProcessPointLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input);
            void ProcessAreaLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input);
            void ProcessMainLight(std::shared_ptr<entt::registry> &registry, RenderStorage &input);

        public:
            using Ptr = std::shared_ptr<LightPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
