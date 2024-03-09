#include "Engine/Render/Pass/IndirectLightPass.h"
#include "Engine/Render/Collections/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void IndirectLightPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/IndirectLightPass.frag");
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          Texture::Format::RGBA, Texture::Format::RGBA32F, Texture::DataType::FLOAT);
            m_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_rt->GetTexture(0)->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            m_rt->Unbind();
        }
        void IndirectLightPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
        }
        void IndirectLightPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            Texture::ResetSlot();
            m_rt->Bind();
            glDisable(GL_DEPTH_TEST);
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            for (auto RTName : RenderStorageInputKey)
            {
                input.Get(RTName)->Activate();
                m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            }
            EngineUtility::DrawQuad();
            m_rt->Unbind();
            glEnable(GL_DEPTH_TEST);
            input.Insert(RenderStorageOutputKey[0], m_rt->GetTexture(0));
        }

    }
}