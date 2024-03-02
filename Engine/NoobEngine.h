#pragma once

#include "Engine/Function/SceneManager.h"
#include "Engine/Function/Buffer/All.h"
#include "Engine/Function/Component/All.h"
#include "Engine/Function/Utility/RayCasting.h"
#include "Engine/Function/Graphics/Material/All.h"
#include "Engine/Function/Graphics/Texture/All.h"
#include "Engine/Function/Graphics/MeshManager.h"
#include "Engine/Function/Graphics/ShaderManager.h"
#include "Engine/Render/All.h"
#include "Engine/Registeration.h"

namespace NoobRenderer
{
    class Engine : public Singleton<Engine>
    {
    public:
        Engine() = delete;
        Engine(token){};
        render::Renderer::Ptr GetRenderer(std::string name);

    public:
        inline void UpdateCurrentRenderer() { m_currentRenderer->Update(); }
        inline void UpdateCurrentScene()
        {
            auto scene = SceneManager::Instance().GetCurrentScene();
            if (scene != nullptr)
                scene->UpdateAllNodesModelMatrix();
        }
        inline void Init()
        {
            Registeration::Init();
            ShaderManager::Instance().Init();
            MeshManager::Instance().Init();
            SceneManager::Instance().Init();
        }
        /**
         * @brief 添加一个渲染器
         * @tparam T Renderer渲染器类型
         * @param name Renderer渲染器名称
         * @param ...args 构造参数
         */
        template <EngineUtility::TisBasedOf<render::Renderer> T, typename... Args>
        inline void AddRenderer(std::string name, Args &&...args)
        {
            auto _tmp = std::make_shared<T>(std::forward<Args>(args)...);
            _tmp->Init();
            m_renderers[name] = _tmp;
        }

        using ForEachRendererFunc = std::function<void(std::string, render::Renderer::Ptr)>;
        inline void ForeachRenderer(ForEachRendererFunc &func)
        {
            for (auto &renderer : m_renderers)
            {
                func(renderer.first, renderer.second);
            }
        }
        inline render::Renderer::Ptr GetCurrentRenderer() { return m_currentRenderer; }
        inline void SetCurrentRenderer(std::string name) { m_currentRenderer = m_renderers[name]; }

    private:
        std::unordered_map<std::string, render::Renderer::Ptr> m_renderers;
        render::Renderer::Ptr m_currentRenderer{nullptr};
    };
}
