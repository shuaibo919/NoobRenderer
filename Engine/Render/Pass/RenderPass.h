#pragma once
#include "Engine/Core/Base.hpp"
#include "Engine/Core/GraphicsAPI.h"
#include "Engine/Core/Utility.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Scene.h"
#include "Engine/Function/Graphics/Shader.h"
#include "Engine/Function/Graphics/ShaderManager.h"
#include "Engine/Function/Utility/Helpers.h"
namespace NoobRenderer
{
    namespace render
    {
        class RenderStorage
        {
        private:
            std::unordered_map<std::string, std::pair<TextureBase::Ptr, int>> m_set;

        public:
            // no check , use catefully!
            TextureBase::Ptr &Get(std::string name) { return m_set[name].first; }
            void Insert(std::string name, TextureBase::Ptr tex, int slot = 0) { m_set[name] = {tex, slot}; }
            inline auto begin() { return m_set.begin(); }
            inline auto end() { return m_set.end(); }
            inline auto size() { return m_set.size(); }
            inline auto empty() { return m_set.empty(); }
            inline auto find(std::string name) { return m_set.find(name); }
        };
        class RenderPass
        {
        protected:
            WriteToTexture::Ptr m_rt{nullptr};

        public:
            using Ptr = std::shared_ptr<RenderPass>;
            virtual void Init(unsigned int width, unsigned int height) = 0;
            virtual void Rescale(unsigned int width, unsigned int height) = 0;
            virtual void Render(Scene::Ptr &scene, RenderStorage &input) = 0;
            WriteToTexture::Ptr &GetRenderTexture() { return m_rt; }
        };

    }
}
