#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class SSRPass : public RenderPass
        {
        private:
            // Todo:
            // ShaderProgram::Ptr m_hiz{nullptr};
            // WriteToTexture::Ptr m_hiz_texture{nullptr};
            ShaderProgram::Ptr m_shader{nullptr};
            unsigned int GetClampedDepth(unsigned int res);
            std::array<std::string, 5> RenderStorageInputKey{
                "uColorMap",
                "uColorBlurMap",
                "gNormal",
                "gMetalRoughAo",
                "gPositionDepth",
            };

        public:
            using Ptr = std::shared_ptr<SSRPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
