#pragma once
#include "Engine/Render/Pass/RenderPass.h"
namespace NoobRenderer
{
    namespace render
    {
        class ForwardPass : public RenderPass
        {
        private:
            ShaderProgram::Ptr m_shader{nullptr};
            ShaderProgram::Ptr m_screen_shader{nullptr};
            ShaderProgram::Ptr m_cubemap_shader{nullptr};
            WriteToTexture::Ptr m_msaa_offline{nullptr};
            void SetLightUniforms(Scene::Ptr &scene, RenderStorage &input);
            void SetSceneUniforms(Scene::Ptr &scene, RenderStorage &input);
            std::array<std::string, 3> RenderStorageInputKey{
                "mainDirLight.shadowMap",
                "pointLightShadowMap[0]",
                "pointLightShadowMap[1]",
            };
            std::array<std::string, 1> RenderStorageOutputKey{
                "Result",
            };

        public:
            using Ptr = std::shared_ptr<ForwardPass>;
            void Init(unsigned int width, unsigned int height) override;
            void Rescale(unsigned int width, unsigned int height) override;

            void Render(Scene::Ptr &scene, RenderStorage &input) override;
            template <EngineUtility::TisBasedOf<TextureBase> T>
            inline bool SetTextureUniform(T::Ptr &tex, ShaderProgram::Ptr &shader, const std::string &name)
            {
                if (tex == nullptr)
                    return false;
                tex->Activate();
                shader->SetUniform(name, static_cast<int>(tex->GetTempSlot()));
                return true;
            }
        };
    }
}
