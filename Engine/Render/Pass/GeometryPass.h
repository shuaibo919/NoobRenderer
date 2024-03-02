#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class GeometryPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader{nullptr};
            std::array<std::string, 6> RenderStorageOutputKey{
                "gPositionDepth",
                "gNormal",
                "gAlbedo",
                "gMetalRoughAo",
                "gEmission",
                "gStencil",
            };

        public:
            using Ptr = std::shared_ptr<GeometryPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;

            /**
             * @brief 渲染
             * @param scene 场景
             * @param RenderStorage None
             * @return RenderStorage:
             * 1-"gPositionDepth",
             * 2-"gNormal",
             * 3-"gAlbedo",
             * 4-"gMetalRoughAo",
             * 5-"gEmission"
             */
            void Render(Scene::Ptr &scene, RenderStorage &input) override;
        };
    }
}
