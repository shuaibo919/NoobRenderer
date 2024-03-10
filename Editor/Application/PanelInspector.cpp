#include "Application/PanelInspector.h"
#include "Application/FontAwesome/IconsFont.h"
using namespace NoobRenderer::Application;
InspectorPanel::InspectorPanel(const std::string &name, bool show) : m_name(name), m_show(show) {}
void InspectorPanel::ListEntityAllComponents()
{
    std::string display_title = IconFont::WithIcon(ICON_FA_VECTOR_SQUARE, "Property");
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.64f);
    if (ImGui::BeginTabItem(display_title.c_str()) && Selected::Instance().node != nullptr)
    {
        /* Reflect scene node properties. */
        if (SceneManager::Instance().GetCurrentSceneNode() == Selected::Instance().node)
        {
            InspectSceneNode();
        }
        /* Reflect all components of a given entity. */
        auto &entity = Selected::Instance().node->object;
        if (entity == nullptr) return;
        auto reg = entity->GetRegistry();
        for (auto &&curr : reg->storage())
        {
            auto &storage = curr.second;
            if (storage.contains(entity->GetEntity()))
            {
                entt::id_type id = curr.first;
                void *componentTypeAny = storage.get(entity->GetEntity());
                inspection::ComponentUIMap::Inspect(entity, id, componentTypeAny);
            }
        }
        /* Add a component for a given entity */
        ImGui::Separator();
        if (ImGui::BeginCombo("Add Component", "", ImGuiComboFlags_None))
        {
            auto &all_components_ctors = ComponentRegisterationMap::GetAllCtorFuncs();
            for (auto &[name, ctor_func] : all_components_ctors)
            {
                if (ImGui::Selectable(name.data()))
                {
                    ctor_func(Selected::Instance().node->object);
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }
    ImGui::EndTabItem();
    ImGui::PopItemWidth();
}
void InspectorPanel::Show()
{
    if (m_show)
        ListEntityAllComponents();
}
void InspectorPanel::InspectSceneNode()
{
    auto &config = SceneManager::Instance().GetCurrentScene()->GetConfig();
    if (ImGui::CollapsingHeader("Scene Setting", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
    {
        // Todo:
        ImGui::Checkbox("Anti-Aliasing", &config.enable_antialias);
        if (ImGui::BeginItemTooltip())
        {
            ImGui::Text("Need to fix: AA does not work!");
            ImGui::EndTooltip();
        }
        // ImGui::Checkbox("Bloom", &config.enable_bloom);
        ImGui::Checkbox("Gamma Correction", &config.enable_gamma_correction);
        ImGui::Checkbox("HDR", &config.enable_hdr);
        ImGui::SliderFloat("Ambient Strength", &config.ambient_strength, 0.0f, 1.0f, "%.2f");
    }
}