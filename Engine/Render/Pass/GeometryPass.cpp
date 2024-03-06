#include "Engine/Render/Pass/GeometryPass.h"
#include "Engine/Render/Pass/Utils.h"
#include <random>
namespace NoobRenderer
{
    namespace render
    {
        void GeometryPass::Init(unsigned int width, unsigned int height)
        {
            m_shader = ShaderManager::Instance().LoadShaderAndGet(
                "Resource/Shader/Deferred/GeometryPass.vert",
                "Resource/Shader/Deferred/GeometryPass.frag");
            using namespace gtype;
            m_rt = std::make_shared<WriteToTexture>(width, height);
            auto gPositionDepthParams = TextureParameter();
            gPositionDepthParams.Add(TexParaType::MinFilter, TexPara::Nearest);
            gPositionDepthParams.Add(TexParaType::MagFilter, TexPara::Nearest);
            gPositionDepthParams.Add(TexParaType::WrapS, TexPara::ClampEdge);
            gPositionDepthParams.Add(TexParaType::WrapT, TexPara::ClampEdge);
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT0, width, height,
                                          gPositionDepthParams, Format::RGBA, Format::RGBA32F, DataType::FLOAT);                   // G-Buffer: gPositionDepth
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT1, width, height);                                                    // G-Buffer: gNormal
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT2, width, height);                                                    // G-Buffer: gAlbedo
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT3, width, height);                                                    // G-Buffer: gMetalRoughAo
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT4, width, height);                                                    // G-Buffer: gEmission
            m_rt->SetTexture2D<Texture2D>(GL_COLOR_ATTACHMENT5, width, height, Format::RGB, Format::RED, DataType::UNSIGNED_BYTE); // G-Buffer: gStencil
            m_rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8);
            m_rt->ExplicitColorAttach();
            m_rt->Unbind();
        }
        void GeometryPass::Rescale(unsigned int width, unsigned int height)
        {
            m_rt->Rescale(width, height);
        }
        void GeometryPass::Render(Scene::Ptr &scene, RenderStorage &input)
        {
            m_rt->Bind();
            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, m_rt->GetWidth(), m_rt->GetHeight());
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            auto &camera = scene->GetCurrentCameraComponent();
            auto registry = scene->GetRegistry();
            auto view = registry->view<component::MeshData>();
            m_shader->SetUniform("view", camera.GetViewMatrix());
            m_shader->SetUniform("projection", camera.GetProjectionMatrix());
            for (auto [entity, mesh_data] : view.each())
            {
                /* Do Transform */
                glm::mat4 model = scene->GetSceneNodeByEntity(entity)->model;
                m_shader->SetUniform("model", model);
                m_shader->SetUniform("invTModel", glm::transpose(glm::inverse(model)));
                auto &mesh_ptr = mesh_data.mesh;
                if (mesh_ptr == nullptr)
                    continue;
                /* Mesh Material Setting */
                auto material = std::dynamic_pointer_cast<PBRMaterial>(mesh_ptr->GetMaterial());
                Texture::ResetSlot();
                auto has_albedo = passutils::SetMaterialUniform(material->albedo, m_shader, "albedo", passutils::MatUniValue::Vec3);
                auto has_emission = passutils::SetMaterialUniform(material->emission, m_shader, "emission", passutils::MatUniValue::Vec3);
                // auto has_ao = passutils::SetMaterialUniform(material->ao, m_shader, "ao", passutils::MatUniValue::Float);
                auto has_metallic = passutils::SetMaterialUniform(material->metallic, m_shader, "metallic", passutils::MatUniValue::Float);
                auto has_roughness = passutils::SetMaterialUniform(material->roughness, m_shader, "roughness", passutils::MatUniValue::Float);
                auto has_normal = passutils::SetMaterialUniform(material->normal, m_shader);
                // auto has_height = SetMaterialUniform(material->height, m_lightShader);
                m_shader->SetUniform("hasAlbedo", static_cast<float>(has_albedo));
                m_shader->SetUniform("hasEmission", static_cast<float>(has_emission));
                m_shader->SetUniform("hasMetallic", static_cast<float>(has_metallic));
                // m_shader->SetUniform("hasAmbientOcclusion", static_cast<float>(has_ao));
                m_shader->SetUniform("hasRoughness", static_cast<float>(has_roughness));
                m_shader->SetUniform("hasNormalMap", static_cast<float>(has_normal));
                m_shader->SetUniform("hasHeightMap", false);
                mesh_ptr->Draw();
            }
            m_rt->Unbind();
            for (int i = 0; i < RenderStorageOutputKey.size(); ++i)
            {
                input.Insert(RenderStorageOutputKey[i], m_rt->GetTexture(i));
            }
        }

    }
}