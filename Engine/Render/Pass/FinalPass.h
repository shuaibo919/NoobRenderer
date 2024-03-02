#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class FinalPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader;

        public:
            using Ptr = std::shared_ptr<FinalPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
