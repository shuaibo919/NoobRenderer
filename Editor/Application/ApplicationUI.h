#pragma once
#include "Engine/NoobEngine.h"
#include "Engine/Window.h"
#include "Application/PanelInspector.h"
#include "Application/PanelScene.h"
#include "Application/PanelViewport.h"
#include "Application/PanelFunction.h"
#include "Application/PanelPreference.h"

namespace NoobRenderer
{
    namespace Application
    {
        class ApplicationUI : public EngineWindow
        {
        public:
            using Ptr = std::shared_ptr<ApplicationUI>;
            ApplicationUI() = delete;
            ApplicationUI(EngineWindow::Size size, std::string_view title, int major_version, int minor_version);
            void UpdateUI();
            bool Init(std::string_view glsl_version);

        public:
            inline EngineWindow::Size GetRenderingSize() { return m_viewport_panel->render_size; }
            inline bool GetReadyToRendering() { return m_viewport_panel->GetReadyToRendering(); }

        private:
            ScenePanel::Ptr m_scene_panel{nullptr};
            InspectorPanel::Ptr m_inspector_panel{nullptr};
            ViewportPanel::Ptr m_viewport_panel{nullptr};
            PreferencePanel::Ptr m_preference_panel{nullptr};
            FunctionPanel::Ptr m_function_panel{nullptr};

        private:
            bool m_show_main_layout{true};
            bool m_show_tabs_panel;
            glm::vec2 m_cursor_position;
            void SetUIStyle();

            void OnResizeCallback(unsigned int width, unsigned int height);
            void OnKeyCallback(int key, int scancode, int action, int mods);
            void OnButtonCallback(int button, int action, int mods);
            void OnCursorPosCallback(double xpos, double ypos);
            void OnScrollCallback(double xoffset, double yoffset);

            void MainLayout();
            void SubMenuScene();
            void SubMenuView();
            void SubMenuRenderer();
            void SubMenuSetting();
            void TabsLayout();
        };

    }
}