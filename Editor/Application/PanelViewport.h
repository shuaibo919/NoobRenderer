#pragma once
#include "Engine/NoobEngine.h"
#include "Engine/Window.h"
#include "Application/Selected.h"
#include "Application/Common.h"
#include "Application/Gizmo/NoobGizmo.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
namespace NoobRenderer
{
    namespace Application
    {
        class ViewportPanel
        {
        private:
            std::string m_name;
            bool m_show, m_show_setting{false};
            bool m_show_grid{false};
            bool m_focused{false};
            bool m_show_pass{false};
            bool m_play_state{false};
            float m_grid_size{100.f};
            int m_pass_visual_column{2};
            ImVec2 ViewportPanelPos;
            ImVec2 ViewportPanelSize;
            void RenderingViewport();
            void ListRenderingButtons();
            void LightHint();
            void SettingPanel();
            void PassVisualizationPanel();
            void FloattingPanel();

        public:
            using Ptr = std::unique_ptr<ViewportPanel>;
            ViewportPanel(const std::string &name, bool show);
            ViewportPanel() = delete;
            void Show();
            bool InRenderingRegion(unsigned int x, unsigned int y);

        public:
            EngineWindow::Size render_size{0, 0};
            unsigned int rendering_pos_x;
            unsigned int rendering_pos_y;
            NoobGizmo::Mode gizmo_mode{NoobGizmo::Mode::TRANSITION};

        public:
            inline bool IsFocused() const { return m_focused; }
            inline bool IsShow() { return m_show; }
            inline void SetShow(bool show) { m_show = show; }
            inline bool GetReadyToRendering() { return render_size.width != 0 && render_size.height != 0; }
        };
    }
}