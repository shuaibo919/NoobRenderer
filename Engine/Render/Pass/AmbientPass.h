#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class AmbientPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader{nullptr};
            ShaderProgram::Ptr m_cubemap_shader{nullptr};
            std::array<std::string, 4> RenderStorageInputKey{
                "gPositionDepth",
                "gNormal",
                "gAlbedo",
                "gMetalRoughAo",
            };
            std::array<std::string, 1> RenderStorageOutputKey{
                "uAmbientColorMap",
            };

        public:
            using Ptr = std::shared_ptr<AmbientPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
