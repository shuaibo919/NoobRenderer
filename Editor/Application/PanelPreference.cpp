#include "Application/PanelPreference.h"
#include "Application/FontAwesome/IconsFont.h"
#include "Engine/NoobEngine.h"
using namespace NoobRenderer::Application;
PreferencePanel::PreferencePanel(const std::string &name, bool show) : m_name(name), m_show(show) {}
void PreferencePanel::Show()
{
    if (m_show)
    {
        auto window_size = ImGui::GetWindowSize();
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
        ImGui::Begin("Preference Setting", &m_show);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.70f);
        {
        }
        ImGui::Checkbox("Show Debug ", &m_show_debug);
        if (m_show_debug)
        {
        }

        ImGui::PopItemWidth();
        ImGui::End();
    }
}
