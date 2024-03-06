#include "Application/Inspector/ModelPreview.h"
#include "Engine/Render/Pass/Utils.h"
using namespace NoobRenderer::inspection;
void ModelPreview::Init()
{
    m_shader = ShaderManager::Instance().LoadShaderAndGet(
        "Resource/Shader/Editor/Preview.vert",
        "Resource/Shader/Editor/Preview.frag");
}
void ModelPreview::Delete(std::string name)
{
    if (m_preview_results.find(name) != m_preview_results.end())
    {
        m_preview_results.erase(name);
    }
}
void ModelPreview::Add(std::string name)
{
    // if (m_preview_results.find(name) != m_preview_results.end())
    //     return;
    // m_preview_results[name] = std::make_shared<WriteToTexture>(m_preview_size, m_preview_size);
    // auto &rt = m_preview_results[name];
    // rt->SetTexture2D<OrdinaryTexture>(GL_COLOR_ATTACHMENT0, m_preview_size, m_preview_size);
    // rt->SetRenderBuffer<RenderBuffer>(GL_DEPTH_ATTACHMENT, m_preview_size, m_preview_size, GL_DEPTH_COMPONENT);
    // std::cout << "ModelPreview RT::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
    // rt->Bind();
    // glViewport(0, 0, m_preview_size, m_preview_size);
    // glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glEnable(GL_DEPTH_TEST);
    // auto box = MeshManager::Instance().GetMeshListBox(name);
    // auto mlists = MeshManager::Instance().GetMeshList(name);
    // m_shader->Use();
    // /* based the bounding box to calculation the mvp */
    // glm::vec3 center = glm::vec3(box[0] + box[3], box[1] + box[4], box[2] + box[5]) * 0.5f;
    // glm::vec3 viewpos = glm::vec3(3.0f, 3.0f, 3.0f);
    // glm::vec3 cameraDirection = glm::normalize(center - viewpos);
    // glm::mat4 view = glm::lookAt(viewpos, center, glm::vec3(0.0f, 1.0f, 0.0f));
    // float aspectRatio = static_cast<float>(m_preview_size) / m_preview_size;
    // glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspectRatio, 0.1f, 1000.0f);
    // float maxDimension = glm::clamp(glm::max(glm::max(box[3] - box[0], box[4] - box[1]), box[5] - box[2]), 0.0f, 100.0f);
    // std::cout << "maxDimension = " << maxDimension << std::endl;
    // float scale = 5.0f / (maxDimension + 1.0f);

    // glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
    // model = glm::scale(model, glm::vec3(scale));

    // m_shader->SetUniform("model", model);
    // m_shader->SetUniform("invTModel", glm::transpose(glm::inverse(model)));
    // m_shader->SetUniform("projection", projection);
    // m_shader->SetUniform("view", view);
    // m_shader->SetUniform("viewPos", viewpos);
    // for (auto mesh : mlists)
    // {
    //     /* Mesh Material Setting */
    //     auto material = std::dynamic_pointer_cast<PBRMaterial>(mesh->GetMaterial());
    //     if (material == nullptr)
    //     {
    //         std::cout << "null" << std::endl;
    //     }
    //     Texture::ResetSlot();
    //     auto has_albedo = render::passutils::SetMaterialUniform(material->albedo, m_shader, "albedo", render::passutils::MatUniValue::Vec3);
    //     auto has_emission = render::passutils::SetMaterialUniform(material->emission, m_shader, "emission", render::passutils::MatUniValue::Vec3);
    //     auto has_metallic = render::passutils::SetMaterialUniform(material->metallic, m_shader, "metallic", render::passutils::MatUniValue::Float);
    //     auto has_roughness = render::passutils::SetMaterialUniform(material->roughness, m_shader, "roughness", render::passutils::MatUniValue::Float);
    //     auto has_normal = render::passutils::SetMaterialUniform(material->normal, m_shader);
    //     m_shader->SetUniform("hasAlbedo", static_cast<float>(has_albedo));
    //     m_shader->SetUniform("hasEmission", static_cast<float>(has_emission));
    //     m_shader->SetUniform("hasMetallic", static_cast<float>(has_metallic));
    //     m_shader->SetUniform("hasRoughness", static_cast<float>(has_roughness));
    //     m_shader->SetUniform("hasNormalMap", static_cast<float>(has_normal));
    //     m_shader->SetUniform("hasHeightMap", false);
    //     mesh->Draw(DrawPrimitive::Triangle);
    // }
    // rt->Unbind();
}