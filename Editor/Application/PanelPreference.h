#pragma once
#include "Application/Selected.h"
#include "Application/Common.h"
#include "Application/Inspector/Inspector.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
namespace NoobRenderer
{
    namespace Application
    {
        class PreferencePanel
        {
        private:
            std::string m_name;
            bool m_show;
            bool m_show_debug;

        public:

            using Ptr = std::unique_ptr<PreferencePanel>;
            void Show();
            inline bool IsShow() { return m_show; }
            inline void SetShow(bool show) { m_show = show; }
            PreferencePanel(const std::string &name, bool show);
            PreferencePanel() = delete;
        };
    }
}