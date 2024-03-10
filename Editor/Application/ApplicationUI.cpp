#include "Application/ApplicationUI.h"
#include "Application/Gizmo/NoobGizmo.h"
#include "stb_image.h"
using namespace NoobRenderer::Application;

ApplicationUI::ApplicationUI(EngineWindow::Size size, std::string_view title, int major_version, int minor_version)
    : EngineWindow(size, title, major_version, minor_version)
{
    GLFWimage icon[1];
    icon[0].pixels = stbi_load("logo.png", &icon[0].width, &icon[0].height, 0, 4);
    glfwSetWindowIcon(m_window, 1, icon);
    stbi_image_free(icon[0].pixels);
}
bool ApplicationUI::Init(std::string_view glsl_version)
{
    // imgui setting
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // Load Arial Font
    ImFontConfig config;
    config.OversampleH = 3;
    config.OversampleV = 2;
    float baseFontSize = 13.0f;
    io.Fonts->AddFontFromFileTTF("Resource/Fonts/arial.ttf", baseFontSize, &config);
    // Load Arial Font
    float iconFontSize = baseFontSize;
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF("Resource/Fonts/fa-solid-900.ttf", iconFontSize, &icons_config, icons_ranges);

    if (!ImGui_ImplGlfw_InitForOpenGL(m_window, true)) // Setup Platform/Renderer backends
    {
        std::cout << "ApplicationUI::ImGui ImplGlfw InitForOpenGL Failed!" << std::endl;
        return false;
    }

    if (!ImGui_ImplOpenGL3_Init(glsl_version.data()))
    {
        std::cout << "ApplicationUI::ImGui ImplOpenGL3 Init Failed!" << std::endl;
        return false;
    }

    SetUIStyle();
    common::imgui_style = ImGui::GetStyle();

    m_scene_panel = std::make_unique<ScenePanel>("SceneList", true);
    m_inspector_panel = std::make_unique<InspectorPanel>("Inspector", true);
    m_viewport_panel = std::make_unique<ViewportPanel>("Viewport", true);
    m_function_panel = std::make_unique<FunctionPanel>("Function", true);
    m_preference_panel = std::make_unique<PreferencePanel>("Preference", false);

    m_outline_pass.Init(m_viewport_panel->render_size.width, m_viewport_panel->render_size.height);

    RegisterOnReSizeFunc(std::bind(&ApplicationUI::OnResizeCallback, this, std::placeholders::_1, std::placeholders::_2));
    RegisterOnKeyFunc(std::bind(&ApplicationUI::OnKeyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    RegisterOnCursorPosFunc(std::bind(&ApplicationUI::OnCursorPosCallback, this, std::placeholders::_1, std::placeholders::_2));
    RegisterOnMouseButtonFunc(std::bind(&ApplicationUI::OnButtonCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    RegisterOnScrollFunc(std::bind(&ApplicationUI::OnScrollCallback, this, std::placeholders::_1, std::placeholders::_2));

    return true;
}
void ApplicationUI::UpdateUI()
{
    // Ui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        MainLayout();
        m_viewport_panel->Show();
        m_scene_panel->Show();
        m_function_panel->Show();
        m_preference_panel->Show();
        TabsLayout();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ApplicationUI::OnResizeCallback(unsigned int width, unsigned int height)
{
    Resize(width, height);
}
void ApplicationUI::OnKeyCallback(int key, int scancode, int action, int mods)
{
    ImGui_ImplGlfw_KeyCallback(m_window, key, scancode, action, mods);
    if (SceneManager::Instance().GetCurrentScene() != nullptr && m_viewport_panel->IsFocused())
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Forward);
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Backward);
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Left);
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Right);
        }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Up);
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move(component::Camera::Action::Down);
        }
    }
}
void ApplicationUI::OnButtonCallback(int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(m_window, button, action, mods);
    if (SceneManager::Instance().GetCurrentScene() != nullptr)
    {
        auto focused = m_viewport_panel->IsFocused();
        if (focused && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().SetMoving(true);
            m_viewport_panel->RayCasting(m_cursor_position.x, m_cursor_position.y);
        }
        if (focused && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().SetMoving(false);
        }
    }
}
void ApplicationUI::OnCursorPosCallback(double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(m_window, xpos, ypos);
    m_cursor_position = glm::vec2(static_cast<int>(xpos), static_cast<int>(ypos));
    if (SceneManager::Instance().GetCurrentScene() != nullptr && !NoobGizmo::IsUsing() && m_viewport_panel->IsFocused())
    {
        if (m_viewport_panel->InRenderingRegion(m_cursor_position.x, m_cursor_position.y))
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Move2D(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}
void ApplicationUI::OnScrollCallback(double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(m_window, xoffset, yoffset);
    if (SceneManager::Instance().GetCurrentScene() != nullptr && !NoobGizmo::IsUsing() && m_viewport_panel->IsFocused())
    {
        if (m_viewport_panel->InRenderingRegion(m_cursor_position.x, m_cursor_position.y))
            SceneManager::Instance().GetCurrentScene()->GetCurrentCameraComponent().Scroll(static_cast<float>(yoffset));
    }
}

void ApplicationUI::SetUIStyle()
{

    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}

/// @brief [1]主界面布局-Main Layout
void ApplicationUI::MainLayout()
{
    ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_DockSpace;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration |
                                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar |
                                    ImGuiConfigFlags_NoMouseCursorChange | ImGuiWindowFlags_NoBringToFrontOnFocus;
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(0, 0.0f), ImGuiCond_Always);
    float f_window_width = static_cast<float>(m_size.width);
    float f_window_height = static_cast<float>(m_size.height);

    ImGui::SetNextWindowSize(ImVec2(f_window_width, f_window_height), ImGuiCond_Always);
    ImGui::SetNextWindowViewport(main_viewport->ID);
    ImGuiID main_docking_id = ImGui::GetID("Main Docking");
    if (ImGui::DockBuilderGetNode(main_docking_id) == nullptr)
    {
        ImGui::DockBuilderRemoveNode(main_docking_id);
        ImGui::DockBuilderAddNode(main_docking_id, dock_flags);
        ImGui::DockBuilderSetNodePos(main_docking_id, ImVec2(0, 10.0f));
        ImGui::DockBuilderSetNodeSize(main_docking_id, ImVec2(f_window_width, f_window_height - 10.0f));

        ImGuiID center = main_docking_id;
        ImGuiID _viewport_panel;
        ImGuiID _tabs_panel = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &_viewport_panel);

        ImGuiID _scene_panel =
            ImGui::DockBuilderSplitNode(_tabs_panel, ImGuiDir_Up, 0.4f, nullptr, &_tabs_panel);

        ImGuiID _function_panel =
            ImGui::DockBuilderSplitNode(_viewport_panel, ImGuiDir_Down, 0.25f, nullptr, &_viewport_panel);

        ImGui::DockBuilderDockWindow("SceneList", _scene_panel);
        ImGui::DockBuilderDockWindow("Tabs", _tabs_panel);
        ImGui::DockBuilderDockWindow("Viewport", _viewport_panel);
        ImGui::DockBuilderDockWindow("Function", _function_panel);

        ImGui::DockBuilderFinish(main_docking_id);
    }

    ImGui::Begin("Editor Menu", &m_show_main_layout, window_flags);
    ImGui::DockSpace(main_docking_id);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Scene"))
        {
            SubMenuScene();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            SubMenuView();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Renderer"))
        {
            SubMenuRenderer();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Setting"))
        {
            SubMenuSetting();
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    ImGui::End();
    // 加载外部场景
    auto getfile = common::ShowFileDiaglogue("Load Scene From File");
    if (getfile.first != "" && getfile.first != "")
    {
        auto id = SceneManager::Instance().CreateFromFileAndAdd(getfile.first);
        SceneManager::Instance().SetCurrentScene(id);
    }
}
/// @brief [1]侧界面页布局-Tabs Layout
void ApplicationUI::TabsLayout()
{
    if (!m_show_tabs_panel)
        return;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
    if (ImGui::Begin("Tabs", &m_show_tabs_panel, window_flags))
    {
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            m_inspector_panel->Show();
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
/// @brief [2]主界面菜单选项-Scene场景
void ApplicationUI::SubMenuScene()
{
    using namespace NoobRenderer;
    if (ImGui::MenuItem("New Scene"))
    {
        Selected::Instance().node = nullptr;
        Scene::Index _id = SceneManager::Instance().CreateEmptyAndAdd("Untitled");
        SceneManager::Instance().SetCurrentScene(_id);
    }
    if (ImGui::MenuItem("From File"))
    {
        Selected::Instance().node = nullptr;
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("Load Scene From File", "Choose File", ".json", config);
    }
}
/// @brief [2]主界面菜单选项-View视图
void ApplicationUI::SubMenuView()
{
    if (ImGui::MenuItem("SceneList Panel"))
    {
        m_scene_panel->SetShow(!m_scene_panel->IsShow());
    }
    if (ImGui::MenuItem("Tabs Panel"))
    {
        m_show_tabs_panel = !m_show_tabs_panel;
    }
}
/// @brief [2]主界面菜单选项-Renderer视图
void ApplicationUI::SubMenuRenderer()
{
    NoobRenderer::Engine::ForEachRendererFunc foreach_renderer = [&](std::string name, NoobRenderer::render::Renderer::Ptr renderer)
    {
        // TODO: Switch Rendere.
        if (ImGui::MenuItem(name.c_str()))
        {
            NoobRenderer::Engine::Instance().SetCurrentRenderer(name);
        }
    };
    NoobRenderer::Engine::Instance().ForeachRenderer(foreach_renderer);
}
/// @brief [2]主界面设置选项-Renderer视图
void ApplicationUI::SubMenuSetting()
{
    if (ImGui::MenuItem("Preference"))
    {
        m_preference_panel->SetShow(!m_preference_panel->IsShow());
    }
}

void ApplicationUI::PostProcessingRender()
{
    m_outline_pass.Render(Selected::Instance().node);
}
void ApplicationUI::PostProcessingRescale()
{
    m_outline_pass.Rescale(this->GetRenderingSize().width, this->GetRenderingSize().height);
}