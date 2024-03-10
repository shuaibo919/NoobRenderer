#pragma once
#include "Engine/NoobEngine.h"
#include "Application/Gizmo/NoobGizmo.h"
#include "Application/Selected.h"
#include "Application/Common.h"
#include "Application/Inspector/ModelPreview.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
namespace NoobRenderer
{
    namespace Application
    {
        class ScenePanel
        {
        private:
            std::string m_name;
            Scene::Node::Ptr m_wait_deleted{nullptr};
            Scene::Node::Ptr m_wait_deleted_parent{nullptr};
            Scene::Ptr m_wait_deleted_scene{nullptr};
            bool m_show;
            void ListAllScenes(Scene::Index idx, Scene::Ptr &scene_node, int &node_cnt);
            void ListAllNodes(Scene::Node::Ptr &node, Scene::Node::Ptr &pre, Scene::Ptr &scene, int &node_cnt, ImGuiTreeNodeFlags flags);
            void ListScenePopupItems(Scene::Ptr &scene_node, Scene::Node::Ptr &root, int &node_cnt);
            void FileDiaglogues(Scene::Ptr &scene_node, Scene::Node::Ptr &root);

        public:
            using Ptr = std::unique_ptr<ScenePanel>;
            void Show();
            ScenePanel(const std::string &name, bool show);
            ScenePanel() = delete;

        public:
            bool IsShow() { return m_show; }
            void SetShow(bool show) { m_show = show; }
        };
    }
}