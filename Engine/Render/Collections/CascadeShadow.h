#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Scene.h"
#include "Engine/Function/Graphics/Shader.h"
namespace NoobRenderer
{
    namespace render
    {
        class CascadeShadow
        {
        private:
            WriteToTexture::Ptr m_map{nullptr};
            UnifromBuffer::Ptr m_csm_buffer{nullptr};
            std::vector<glm::mat4> m_light_space_matrices;
            int m_split_size;
            glm::mat4 CalcLightSpace(Scene::Ptr &scene, component::DirectionalLight &light, float near, float far);
            void CalcLightSpaceMatrices(Scene::Ptr &scene, component::DirectionalLight &light);

        public:
            using Ptr = std::shared_ptr<CascadeShadow>;
            CascadeShadow() = delete;
            CascadeShadow(unsigned int width, unsigned int height, int split_size, unsigned int binding = 0);
            std::vector<float> GetCascadeLevels(std::vector<float>& cascade_splits, float camera_far_plane);
            ~CascadeShadow();
            void WrtieToMap(ShaderProgram::Ptr shader, Scene::Ptr &scene, component::DirectionalLight &light);
            inline void ActivateTex() { m_map->ActivateTex(); }
            inline int GetTexSlot() { return m_map->GetTexSlot(); }
            inline TextureBase::Ptr &GetTexture() { return m_map->GetTexture(); }
            inline std::vector<glm::mat4> &GetLightSpaceMatrices() { return m_light_space_matrices; }
        };
    }
}
