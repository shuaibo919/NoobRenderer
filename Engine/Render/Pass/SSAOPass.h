#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class SSAOPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader{nullptr};
            ShaderProgram::Ptr m_blur{nullptr};
            Texture2D::Ptr m_noise{nullptr};
            WriteToTexture::Ptr m_ssao{nullptr};
            std::vector<glm::vec3> m_kernel;
            std::array<std::string, 2> RenderStorageInputKey{
                "gPositionDepth",
                "gNormal",
            };
            std::array<std::string, 2> RenderStorageOutputKey{
                "uSsaoNoise",
                "uSsaoMap",
            };

        public:
            using Ptr = std::shared_ptr<SSAOPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
