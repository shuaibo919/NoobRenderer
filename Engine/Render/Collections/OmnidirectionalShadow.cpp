#include "Engine/Render/Collections/OmnidirectionalShadow.h"
namespace NoobRenderer
{
    namespace render
    {
        OmnidirectionalShadow::OmnidirectionalShadow(unsigned int width, unsigned int height)
        {

            m_map = std::make_shared<WriteToTexture>(width, height);
            auto texParams = TextureParameter();
            texParams.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Nearest);
            texParams.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Nearest);
            texParams.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampBorder);
            texParams.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampBorder);
            texParams.Add(gtype::TexParaType::WrapR, gtype::TexPara::ClampBorder);
            GLfloat borderColor[] = {0.0, 0.0, 0.0, 1.0};
            m_map->SetTexture<CubeMapTexture>(GL_DEPTH_ATTACHMENT, width, height, gtype::Format::DEPTH_COMPONENT, gtype::Format::DEPTH_COMPONENT, gtype::DataType::FLOAT, texParams);
            m_map->GetTexture()->SetParameter(gtype::TexParaType::BorderColor, borderColor);
            m_map->DiscardRenderBuffer();
            std::cout << "OmnidirectionalShadow::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_map->Unbind();
        }
        OmnidirectionalShadow::~OmnidirectionalShadow()
        {
            m_map.reset();
        }
        void OmnidirectionalShadow::WrtieToMap(ShaderProgram::Ptr shader, Scene::Ptr &scene, component::PointLight &light)
        {
            glViewport(0, 0, m_map->GetWidth(), m_map->GetHeight());
            m_map->Bind();
            // m_map->ExplicitColorAttach();
            shader->Use();
            glClear(GL_DEPTH_BUFFER_BIT);
            // glCullFace(GL_FRONT); // peter panning
            scene->BasicRender(shader);
            // glCullFace(GL_BACK);
            m_map->Unbind();
            // glEnable(GL_CULL_FACE);
        }
        std::vector<glm::mat4> OmnidirectionalShadow::GetLightSpace(component::PointLight &light)
        {
            float aspect = (float)m_map->GetWidth() / (float)m_map->GetHeight();
            glm::mat4 proj = glm::perspective(glm::radians(90.0f), aspect, light.near, light.far);
            std::vector<glm::mat4> transforms;
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
            transforms.push_back(proj * glm::lookAt(light.position, light.position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
            return transforms;
        }

    }
}