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
        class OmnidirectionalShadow
        {
        private:
            WriteToTexture::Ptr m_map{nullptr};

        public:
            using Ptr = std::shared_ptr<OmnidirectionalShadow>;
            OmnidirectionalShadow() = delete;
            OmnidirectionalShadow(unsigned int width, unsigned int height);
            ~OmnidirectionalShadow();
            void WrtieToMap(ShaderProgram::Ptr shader, Scene::Ptr &scene, component::PointLight &light);
            inline void ActivateTex() { m_map->ActivateTex(); }
            inline int GetTexSlot() { return m_map->GetTexSlot(); }
            inline GLint GetTexId() { return m_map->GetTexId(); }
            inline TextureBase::Ptr &GetTexture() { return m_map->GetTexture(); }
            std::vector<glm::mat4> GetLightSpace(component::PointLight &light);
        };
    }
}
