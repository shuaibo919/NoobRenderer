#include "Application/PanelViewport.h"
#include "Application/Gizmo/ImGuizmo.h"
using namespace NoobRenderer::Application;
ViewportPanel::ViewportPanel(const std::string &name, bool show) : m_name(name), m_show(show) {}
void ViewportPanel::RenderingViewport()
{
    using namespace NoobRenderer;
    auto renderer = Engine::Instance().GetCurrentRenderer();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    ImGui::Begin(m_name.c_str(), nullptr, window_flags);
    m_focused = ImGui::IsWindowFocused();
    ViewportPanelPos = ImGui::GetWindowPos();
    ViewportPanelSize = ImGui::GetWindowSize();
    rendering_pos_x = static_cast<unsigned int>(ViewportPanelPos.x + common::imgui_style.WindowPadding.x);
    rendering_pos_y = static_cast<unsigned int>(ViewportPanelPos.y + ImGui::GetCurrentWindow()->TitleBarHeight() + common::imgui_style.WindowPadding.y);
    render_size.width = static_cast<unsigned int>(ImGui::GetContentRegionAvail().x);
    render_size.height = static_cast<unsigned int>(ImGui::GetContentRegionAvail().y);
    if (renderer != nullptr)
    {
        auto tex = renderer->GetRenderResult();
        if (tex != -1)
        {
            // 叠加在Rendering窗口上
            ImGui::GetWindowDrawList()->AddImage((ImTextureID)(intptr_t)(tex),
                                                 ImVec2(static_cast<float>(rendering_pos_x), static_cast<float>(rendering_pos_y)),
                                                 ImVec2(rendering_pos_x + ImGui::GetContentRegionAvail().x, rendering_pos_y + ImGui::GetContentRegionAvail().y),
                                                 ImVec2(0, 1), ImVec2(1, 0));
            ImGui::GetWindowDrawList()->AddText(
                ImVec2(static_cast<float>(render_size.width) * 0.95f, static_cast<float>(rendering_pos_y)),
                ImGui::GetColorU32(ImGuiCol_Text),
                ("FPS: " + std::to_string(ImGui::GetIO().Framerate)).c_str());

            // ImGui::Image((ImTextureID)(intptr_t)(tex), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
            NoobGizmo::SetRegion(rendering_pos_x, rendering_pos_y, render_size.width, render_size.height);
            if (m_show_grid)
                NoobGizmo::ShowGrid(m_grid_size);
            if (Selected::Instance().node != nullptr)
            {
                NoobGizmo::ShowGizmo(gizmo_mode, Selected::Instance().node);
                LightHint();
            }
        }
    }
    ListRenderingButtons();

    ImGui::End();
}
void ViewportPanel::LightHint()
{
    using namespace NoobRenderer;
    auto current_scene = SceneManager::Instance().GetCurrentScene();
    auto current_scene_reg = current_scene->GetRegistry();
    auto view = current_scene_reg->view<component::PointLight>();
    for (auto [entity, light] : view.each())
    {
        if (gizmo_mode != NoobGizmo::Mode::TRANSITION)
            continue;

        if (NoobGizmo::ShowLightHint(light.position) && !current_scene_reg->any_of<component::MeshData>(entity))
        {
            current_scene->GetSceneNodeByEntity(entity, Selected::Instance().node);
        }
    }
}
void ViewportPanel::ListRenderingButtons()
{
    static const char *button_icons[] = {
        ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, ICON_FA_ARROWS_SPIN, ICON_FA_ARROWS_LEFT_RIGHT_TO_LINE, ICON_FA_ARROW_POINTER,
        ICON_FA_CODE, ICON_FA_GEAR};
    static const char *button_tips[] = {
        "Translate Mode", "Rotate Mode", "Scale Mode", "Select Mode", "Pass Mode", "ViewPort Setting"};
    static const std::function<void(void)> button_funcs[] = {
        [&]()
        { gizmo_mode = NoobGizmo::Mode::TRANSITION; },
        [&]()
        { gizmo_mode = NoobGizmo::Mode::ROTATE; },
        [&]()
        { gizmo_mode = NoobGizmo::Mode::SCALING; },
        [&]()
        { gizmo_mode = NoobGizmo::Mode::SELECT; },
        [&]()
        { m_show_pass = !m_show_pass; },
        [&]()
        { m_show_setting = !m_show_setting; }};
    for (int i = 0; i < 6; i++)
    {
        if (i > 0)
            ImGui::SameLine();
        ImGui::PushID(i);
        if (ImGui::Button(button_icons[i]))
        {
            button_funcs[i]();
        }
        ImGui::SetItemTooltip(button_tips[i]);
        ImGui::PopID();
    }
}
void ViewportPanel::Show()
{
    if (m_show)
    {
        RenderingViewport();
        // FloattingPanel();
        SettingPanel();
        PassVisualizationPanel();
    }
}
bool ViewportPanel::InRenderingRegion(unsigned int x, unsigned int y)
{
    return (x >= rendering_pos_x) && (x < (rendering_pos_x + render_size.width)) && (y >= rendering_pos_y) && (y < (rendering_pos_y + render_size.height));
}
void ViewportPanel::SettingPanel()
{
    if (m_show_setting)
    {
        auto window_size = ImGui::GetWindowSize();
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
        ImGui::Begin("Rendering Setting", &m_show_setting);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.70f);
        {
            // 网格线渲染
            ImGui::Checkbox("Show Grid", &m_show_grid);
            if (m_show_grid)
            {
                ImGui::DragFloat("Grid Size", &m_grid_size, 1.0f, 1.0f, 500.f);
            }
            ImGui::SliderInt("Pass Column", &m_pass_visual_column, 1, 4);
        }

        ImGui::PopItemWidth();
        ImGui::End();
    }
}
void ViewportPanel::PassVisualizationPanel()
{
    if (m_show_pass)
    {
        auto window_size = ImGui::GetWindowSize();
        ImGui::SetNextWindowSize(ImVec2(window_size.x, window_size.y), ImGuiCond_Once);
        ImGui::Begin("Pass Visualization", &m_show_setting);
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.70f);
        {
            auto renderer = NoobRenderer::Engine::Instance().GetCurrentRenderer();
            auto region_avail = ImGui::GetContentRegionAvail();
            if (renderer != nullptr)
            {
                std::vector<std::pair<std::string, GLuint>> tmp_key_texs;
                for (auto &[key, tex_desc] : renderer->RTS)
                {
                    if (tex_desc.second != 0) // now only support 2d render texs
                        continue;
                    tmp_key_texs.push_back({key, tex_desc.first->GetID()});
                }
                for (int i = 0; i < tmp_key_texs.size(); i += m_pass_visual_column)
                {
                    if (ImGui::BeginTable("PassVisualization", m_pass_visual_column, ImGuiTableFlags_BordersV))
                    {
                        for (int column = 0; ((i + column) < tmp_key_texs.size()) && (column < m_pass_visual_column); column++)
                        {
                            ImGui::TableSetupColumn(tmp_key_texs[i + column].first.c_str());
                        }
                        ImGui::TableHeadersRow();

                        ImGui::TableNextRow();
                        for (int column = 0; ((i + column) < tmp_key_texs.size()) && (column < m_pass_visual_column); column++)
                        {
                            ImGui::TableSetColumnIndex(column);
                            ImGui::ImageButton((ImTextureID)(intptr_t)(tmp_key_texs[i + column].second), region_avail / static_cast<float>(m_pass_visual_column + 0.1f), ImVec2(0, 1), ImVec2(1, 0));
                        }

                        ImGui::EndTable();
                    }
                }
            }
        }

        ImGui::PopItemWidth();
        ImGui::End();
    }
}
void ViewportPanel::FloattingPanel()
{
    ImVec2 window_pos = ImVec2(ViewportPanelPos.x + ViewportPanelSize.x * 0.02f, ViewportPanelPos.y + ViewportPanelSize.y * 0.94f);
    ImVec2 window_size = ImVec2(ViewportPanelSize.x * 0.96f, ViewportPanelSize.y * 0.04f);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::Begin("Floating Panel", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    auto paly_button_icon = m_play_state ? ICON_FA_PAUSE : ICON_FA_PLAY;
    ImVec2 button_size = ImVec2(window_size.x * 0.025f, window_size.y * 0.55f);
    ImGuiStyle &style = ImGui::GetStyle();
    float width = 0.0f;
    width += button_size.x;
    common::AlignForWidth(width);

    if (ImGui::Button(paly_button_icon, button_size))
    {
        m_play_state = !m_play_state;
    }
    ImGui::SameLine();

    ImGui::End();
}