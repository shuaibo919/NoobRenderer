#include "Engine/Render/Collections/CascadeShadow.h"
namespace NoobRenderer
{
    namespace render
    {
        CascadeShadow::CascadeShadow(unsigned int width, unsigned int height, int split_size, unsigned int binding)
            : m_split_size(split_size)
        {
            using namespace Texture;
            GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
            m_map = std::make_shared<WriteToTexture>(width, height);
            m_map->SetTexture<Texture2DArray>(GL_DEPTH_ATTACHMENT, width, height, m_split_size,
                                              Format::DEPTH_COMPONENT, Format::DEPTH_COMPONENT, DataType::FLOAT);
            m_map->GetTexture(0)->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_map->GetTexture(0)->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            m_map->GetTexture(0)->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            m_map->GetTexture(0)->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            m_map->GetTexture(0)->SetParameter(GL_TEXTURE_BORDER_COLOR, borderColor);
            m_map->DiscardRenderBuffer();
            std::cout << "CascadeShadow::Constructor() status = " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
            m_map->Unbind();
            m_csm_buffer = std::make_shared<UnifromBuffer>(sizeof(glm::mat4) * 12, binding);
            m_csm_buffer->Unbind();
            m_light_space_matrices.resize(m_split_size);
        }
        void CascadeShadow::WrtieToMap(ShaderProgram::Ptr shader, Scene::Ptr &scene, component::DirectionalLight &light)
        {
            CalcLightSpaceMatrices(scene, light);
            m_csm_buffer->Bind();
            for (size_t i = 0; i < m_light_space_matrices.size(); ++i)
            {
                m_csm_buffer->SubData(i * sizeof(glm::mat4), sizeof(glm::mat4), &m_light_space_matrices[i]);
            }
            m_csm_buffer->Unbind();
            m_map->Bind();
            shader->Use();
            {
                auto program = shader->GetShaderProgram();
                GLuint blockIndex = glGetUniformBlockIndex(program, "LightSpaceMatrices");
                glUniformBlockBinding(program, blockIndex, m_csm_buffer->GetBinding());
            }
            glViewport(0, 0, m_map->GetWidth(), m_map->GetHeight());
            glClear(GL_DEPTH_BUFFER_BIT);
            scene->BasicRender(shader);
            m_map->Unbind();
        }
        std::vector<float> CascadeShadow::GetCascadeLevels(std::vector<float> &cascade_splits, float camera_far_plane)
        {
            std::vector<float> answer(m_split_size, camera_far_plane);
            for (int i = 0; i < answer.size(); ++i)
                answer[i] /= cascade_splits[i];
            return answer;
        }
        glm::mat4 CascadeShadow::CalcLightSpace(Scene::Ptr &scene, component::DirectionalLight &light, float near, float far)
        {
            auto &camera = scene->GetCurrentCameraComponent();
            auto cacasde_projection = glm::perspective(glm::radians(camera.m_field_of_view), camera.m_aspect, near, far);
            std::vector<glm::vec4> corners = EngineUtility::GetFrustumCornersWorldSpace(cacasde_projection, camera.GetViewMatrix());

            glm::vec3 center = glm::vec3(0, 0, 0);
            for (const auto &v : corners)
            {
                center += glm::vec3(v);
            }
            center /= corners.size();

            glm::vec3 normlized_direction = glm::normalize(light.direction);
            glm::mat4 lightView = glm::lookAt(center + normlized_direction, center, glm::vec3(0.0f, 1.0f, 0.0f));

            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::lowest();
            for (const auto &v : corners)
            {
                const auto trf = lightView * v;
                minX = std::min(minX, trf.x);
                maxX = std::max(maxX, trf.x);
                minY = std::min(minY, trf.y);
                maxY = std::max(maxY, trf.y);
                minZ = std::min(minZ, trf.z);
                maxZ = std::max(maxZ, trf.z);
            }

            // Tune this parameter according to the scene
            constexpr float zMult = 10.0f;
            if (minZ < 0)
            {
                minZ *= zMult;
            }
            else
            {
                minZ /= zMult;
            }
            if (maxZ < 0)
            {
                maxZ /= zMult;
            }
            else
            {
                maxZ *= zMult;
            }
            glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
            return lightProjection * lightView;
        }
        void CascadeShadow::CalcLightSpaceMatrices(Scene::Ptr &scene, component::DirectionalLight &light)
        {
            float near = scene->GetCurrentCameraComponent().GetNear();
            float far = scene->GetCurrentCameraComponent().GetFar();
            for (size_t i = 0; i < m_light_space_matrices.size(); ++i)
            {
                if (i == 0)
                {
                    m_light_space_matrices[i] = CalcLightSpace(scene, light, near, far / light.cascade_splits[i]);
                }
                else
                {
                    m_light_space_matrices[i] = CalcLightSpace(scene, light, far / light.cascade_splits[i - 1], far / light.cascade_splits[i]);
                }
            }
        }
        CascadeShadow::~CascadeShadow()
        {
            m_map.reset();
            m_csm_buffer.reset();
        }
    }
}