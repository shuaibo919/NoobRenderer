#include "Application/Gizmo/NoobGizmo.h"
#include "Application/Gizmo/ImGuizmo.h"
#include "Application/FontAwesome/IconsFont.h"
#include "Third/glm/gtx/matrix_decompose.hpp"
#define GLM_ENABLE_EXPERIMENTAL
void NoobGizmo::ShowGizmo(NoobGizmo::Mode mode, NoobRenderer::Scene::Node::Ptr &node)
{
    using namespace NoobRenderer;
    auto &camera_node = SceneManager::Instance().GetCurrentScene()->GetCurrentCamera()->object->GetComponent<component::Camera>();
    auto camera_view = camera_node.GetViewMatrix();
    auto camera_projection = camera_node.GetProjectionMatrix();
    switch (mode)
    {
    case NoobGizmo::Mode::TRANSITION:
        ManipulateTransition(node, camera_view, camera_projection);
        break;
    case NoobGizmo::Mode::ROTATE:
        ManipulateRotate(node, camera_view, camera_projection);
        break;
    case NoobGizmo::Mode::SCALING:
        ManipulateScaling(node, camera_view, camera_projection);
        break;

    default:
        break;
    }
}
void NoobGizmo::SetRegion(unsigned int posx, unsigned int posy, unsigned int width, unsigned int height)
{
    NoobGizmo::gizmo_rect_x = static_cast<float>(posx);
    NoobGizmo::gizmo_rect_y = static_cast<float>(posy);
    NoobGizmo::gizmo_rect_width = static_cast<float>(width);
    NoobGizmo::gizmo_rect_height = static_cast<float>(height);
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(gizmo_rect_x, gizmo_rect_y, gizmo_rect_width, gizmo_rect_height);
}
void NoobGizmo::ManipulateTransition(NoobRenderer::Scene::Node::Ptr &node, glm::mat4 &view, glm::mat4 &projection)
{
    using namespace NoobRenderer;
    if (node->object->HasComponent<component::Transform>())
    {
        auto &transform = node->object->GetComponent<component::Transform>();
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE,
                             ImGuizmo::LOCAL, glm::value_ptr(transform.model));
        if (ImGuizmo::IsUsing())
        {
            transform.position = EngineUtility::DecomposePositonFromMatrix(transform.model);
        }
    }
    if (node->object->HasComponent<component::PointLight>())
    {
        auto &light = node->object->GetComponent<component::PointLight>();
        glm::mat4 t = glm::translate(glm::mat4(1.0f), light.position);
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::TRANSLATE,
                             ImGuizmo::LOCAL, glm::value_ptr(t), nullptr, nullptr, nullptr, nullptr, false);
        if (ImGuizmo::IsUsing())
        {
            light.position = EngineUtility::DecomposePositonFromMatrix(t);
        }
    }
}
void NoobGizmo::ManipulateScaling(NoobRenderer::Scene::Node::Ptr &node, glm::mat4 &view, glm::mat4 &projection)
{
    using namespace NoobRenderer;
    if (node->object->HasComponent<component::Transform>())
    {
        auto &transform = node->object->GetComponent<component::Transform>();
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::SCALE,
                             ImGuizmo::LOCAL, glm::value_ptr(transform.model));

        if (ImGuizmo::IsUsing())
        {
            transform.scale = EngineUtility::DecomposeScaleFromMatrix(transform.model);
        }
    }
}
void NoobGizmo::ManipulateRotate(NoobRenderer::Scene::Node::Ptr &node, glm::mat4 &view, glm::mat4 &projection)
{
    using namespace NoobRenderer;
    if (node->object->HasComponent<component::Transform>())
    {
        auto &transform = node->object->GetComponent<component::Transform>();
        glm::mat4 tmpMatrix = glm::mat4(1.0f);
        ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transform.position), glm::value_ptr(transform.rotate), glm::value_ptr(transform.scale), glm::value_ptr(tmpMatrix));
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), ImGuizmo::OPERATION::ROTATE,
                             ImGuizmo::WORLD, glm::value_ptr(tmpMatrix));
        if (ImGuizmo::IsUsing())
        {
            ImGuizmo::DecomposeMatrixToRotation(glm::value_ptr(tmpMatrix), glm::value_ptr(transform.rotate));
        }
    }
}
void NoobGizmo::ShowGrid(float grid_size)
{
    using namespace NoobRenderer;
    auto current_scene = SceneManager::Instance().GetCurrentScene();
    if (current_scene != nullptr)
    {
        auto &camera_node = current_scene->GetCurrentCamera()->object->GetComponent<component::Camera>();
        auto camera_view = camera_node.GetViewMatrix();
        auto camera_projection = camera_node.GetProjectionMatrix();
        ImGuizmo::DrawGrid(glm::value_ptr(camera_view), glm::value_ptr(camera_projection), glm::value_ptr(glm::mat4(1.0f)), grid_size);
    }
}
bool NoobGizmo::ShowLightHint(glm::vec3 position)
{
    using namespace NoobRenderer;
    auto &camera_node = SceneManager::Instance().GetCurrentScene()->GetCurrentCamera()->object->GetComponent<component::Camera>();
    auto viewProjection = camera_node.GetProjectionMatrix() * camera_node.GetViewMatrix();
    auto screen_light_pos = viewProjection * glm::vec4(position, 1.0f);
    if (screen_light_pos.z < 0.00001f)
        return false;
    screen_light_pos /= screen_light_pos.w;
    ImVec2 pos_rendc = ImVec2((screen_light_pos.x + 1) * gizmo_rect_width / 2.f, (1 - screen_light_pos.y) * gizmo_rect_height / 2.f);
    if (pos_rendc.x > gizmo_rect_width || pos_rendc.y > gizmo_rect_height)
        return false;
    if (pos_rendc.x < 0 || pos_rendc.y < 0)
        return false;
    bool hovered = ImGui::IsMouseHoveringRect(ImVec2(gizmo_rect_x + pos_rendc.x - 8.0f, gizmo_rect_y + pos_rendc.y - 8.0f),
                                              ImVec2(gizmo_rect_x + pos_rendc.x + 8.0f, gizmo_rect_y + pos_rendc.y + 8.0f));

    ImColor color = hovered ? ImColor(64, 64, 128) : ImColor(245, 245, 245);
    ImGui::GetForegroundDrawList()
        ->AddText(NULL, 16.0f, ImVec2(gizmo_rect_x + pos_rendc.x - 8.0f, gizmo_rect_y + pos_rendc.y - 8.0f),
                  color, ICON_FA_LIGHTBULB);

    return hovered && ImGui::IsMouseClicked(0);
}
bool NoobGizmo::IsUsing()
{
    return ImGuizmo::IsUsingAny();
}