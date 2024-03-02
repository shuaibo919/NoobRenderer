#pragma once
#include "Application/Selected.h"
#include "Application/Common.h"
#include "Application/Inspector/Inspector.h"
#include "Application/Inspector/ModelPreview.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
namespace NoobRenderer
{
    namespace Application
    {
        class FunctionPanel
        {
        private:
            std::unordered_map<std::string, OrdinaryTexture::Ptr> m_resource_icons;
            std::string m_name;
            bool m_show;
            bool m_show_debug;
            void ResourcePanel();
            void ConsolePanel();
            void FileDiaglogues();

        public:
            using Ptr = std::unique_ptr<FunctionPanel>;
            void Show();
            inline bool IsShow() { return m_show; }
            inline void SetShow(bool show) { m_show = show; }
            FunctionPanel(const std::string &name, bool show);
            FunctionPanel() = delete;
        };
    }
}