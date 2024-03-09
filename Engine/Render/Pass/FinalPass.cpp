#include "Engine/Render/Pass/FinalPass.h"
#include "Engine/Render/Collections/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void FinalPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/FinalPass.frag");
            using namespace Texture;
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          Texture::Format::RGBA, Texture::Format::RGB16F, Texture::DataType::FLOAT);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            std::cout << "FinalPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_rt->Unbind();
        }
        void FinalPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
        }
        void FinalPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            Texture::ResetSlot();
            m_rt->Bind();
            glDisable(GL_DEPTH_TEST);
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            Texture::ResetSlot();
            auto &light_pass_result = input.Get("uMixedColorMap");
            light_pass_result->Activate();
            m_shader->SetUniform("screenTexture", light_pass_result->GetTempSlot());
            m_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            m_shader->SetUniform("enableAntiAliasing", scene->GetConfig().enable_antialias);
            m_shader->SetUniform("enableHDR", scene->GetConfig().enable_hdr);
            m_shader->SetUniform("enableGammaCorrection", scene->GetConfig().enable_gamma_correction);
            EngineUtility::DrawQuad();
            m_rt->Unbind();
            glEnable(GL_DEPTH_TEST);
            input.Insert("Result", m_rt->GetTexture(0));
        }

    }
}