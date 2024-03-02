#pragma once
#include "Engine/NoobEngine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

// NoobGizmo is based on ImGuizmo: https://github.com/CedricGuillemet/ImGuizmo

class NoobGizmo
{
public:
    enum class Mode
    {
        TRANSITION,
        SCALING,
        ROTATE
    };

private:
    inline static float gizmo_rect_x{0};
    inline static float gizmo_rect_y{0};
    inline static float gizmo_rect_width{0};
    inline static float gizmo_rect_height{0};
    static void ManipulateTransition(NoobRenderer::Scene::Node::Ptr &node);
    static void ManipulateScaling(NoobRenderer::Scene::Node::Ptr &node);
    static void ManipulateRotate(NoobRenderer::Scene::Node::Ptr &node);

public:
    static void SetRegion(unsigned int posx, unsigned int posy, unsigned int width, unsigned int height);
    static void ShowGizmo(NoobGizmo::Mode mode, NoobRenderer::Scene::Node::Ptr &node);
    static void ShowGrid(float grid_size);
    static bool ShowLightHint(glm::vec3 position);

    static bool IsUsing();
};
