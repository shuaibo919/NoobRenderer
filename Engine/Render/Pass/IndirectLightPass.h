#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class IndirectLightPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader;
            std::array<std::string, 5> RenderStorageInputKey{
                "uColorMap",
                "uAmbientColorMap",
                "uSsaoMap",
                "uSsrMap",
                "uVolumetric",
            };
            std::array<std::string, 1> RenderStorageOutputKey{
                "uMixedColorMap",
            };

        public:
            using Ptr = std::shared_ptr<IndirectLightPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
