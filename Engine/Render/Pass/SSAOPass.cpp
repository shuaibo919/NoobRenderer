#include "Engine/Render/Pass/SSAOPass.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void SSAOPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/SSAOPass.frag");
            m_blur = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/Screen.vert",
                "Resource/Shader/Deferred/SSAOBlurPass.frag");
            m_rt = std::make_shared<WriteToTexture>(width, height);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height, gtype::Format::RGB, gtype::Format::RED, gtype::DataType::FLOAT);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_rt->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            std::cout << "SSAOPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_rt->Unbind();

            m_ssao = std::make_shared<WriteToTexture>(width, height);
            m_ssao->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height, gtype::Format::RGB, gtype::Format::RED, gtype::DataType::FLOAT);
            m_ssao->GetTexture()->SetParameterAndSave(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_ssao->GetTexture()->SetParameterAndSave(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_ssao->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT);
            std::cout << "SSAOPass RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_ssao->Unbind();
            // Sample kernel
            std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
            std::default_random_engine generator;
            for (GLuint i = 0; i < 64; ++i)
            {
                glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
                sample = glm::normalize(sample);
                sample *= randomFloats(generator);
                GLfloat scale = GLfloat(i) / 64.0f;

                // Scale samples s.t. they're more aligned to center of kernel
                scale = EngineUtility::lerp(0.1f, 1.0f, scale * scale);
                sample *= scale;
                m_kernel.push_back(sample);
            }
            // Noise texture
            std::vector<glm::vec3> ssaoNoise;
            for (GLuint i = 0; i < 16; i++)
            {
                glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
                ssaoNoise.push_back(noise);
            }
            m_noise = std::make_shared<Texture2D>(4, 4, &ssaoNoise[0], gtype::Format::RGB, gtype::Format::RGB16F, gtype::DataType::FLOAT);
            m_ssao->GetTexture()->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_ssao->GetTexture()->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_ssao->GetTexture()->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
            m_ssao->GetTexture()->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
            m_noise->Unbind();
        }
        void SSAOPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
            m_ssao->Rescale(width, height);
        }
        void SSAOPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            auto &camera = scene->GetCurrentCameraComponent();

            m_rt->Bind();
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT);
            m_shader->Use();
            Texture::ResetSlot();
            for (auto RTName : RenderStorageInputKey)
            {
                input.Get(RTName)->Activate();
                m_shader->SetUniform(RTName, input.Get(RTName)->GetTempSlot());
            }
            m_noise->Activate();
            m_shader->SetUniform("mNoise", m_noise->GetTempSlot());
            m_shader->SetUniform("near", camera.GetNear());
            m_shader->SetUniform("far", camera.GetFar());
            m_shader->SetUniform("projection", camera.GetProjectionMatrix());
            m_shader->SetUniform("view", camera.GetViewMatrix());
            m_shader->SetUniform("invTView", glm::transpose(glm::inverse(camera.GetViewMatrix())));
            m_shader->SetUniform("viewportSize", glm::vec2(m_rt->GetWidth(), m_rt->GetHeight()));
            for (int i = 0; i < 64; i++)
            {
                m_shader->SetUniform("samples[" + std::to_string(i) + "]", m_kernel[i]);
            }
            EngineUtility::DrawQuad();
            m_rt->Unbind();

            m_ssao->Bind();
            Texture::ResetSlot();
            m_rt->ActivateTex();
            m_blur->SetUniform("mSsaoMap", m_rt->GetTexSlot());
            EngineUtility::DrawQuad();
            m_ssao->Unbind();
            input.Insert(RenderStorageOutputKey[0], m_noise);
            input.Insert(RenderStorageOutputKey[1], m_ssao->GetTexture());
        }

    }
}