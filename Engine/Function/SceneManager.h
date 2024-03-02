#pragma once
#include "Engine/Function/Scene.h"
namespace NoobRenderer
{
    /**
     * @brief 场景管理器(recommended)
     */
    class SceneManager : public Singleton<SceneManager>
    {
    public:
        SceneManager() = delete;
        SceneManager(token) {}
        void Init();
        void AddScene(Scene::Ptr &scene);
        Scene::Index CreateFromFileAndAdd(std::string path);
        Scene::Index CreateEmptyAndAdd(std::string &name);
        Scene::Index CreateEmptyAndAdd(std::string_view name);
        Scene::Ptr GetCurrentScene();
        Scene::Node::Ptr &GetCurrentSceneNode();
        Scene::Ptr GetScene(Scene::Index idx);
        void DeleteScene(Scene::Index idx);
        void DeleteScene(Scene::Ptr &ptr);
        void SetCurrentScene(Scene::Index idx);

    public:
        inline auto begin() { return m_scenes.begin(); }
        inline auto end() { return m_scenes.end(); }

    private:
        std::unordered_map<Scene::Index, Scene::Ptr> m_scenes;
        Scene::Index m_current_count{0};
        Scene::Ptr m_current_scene{nullptr};
    };
} // namespace NoobRenderer
