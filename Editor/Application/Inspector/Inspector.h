#pragma once
#include "Application/Inspector/Registration.h"
#include "Application/FileBrowser/ImGuiFileDialog.h"
#include "Application/FontAwesome/IconsFont.h"
#include "Engine/NoobEngine.h"

// To wire InputText() with std::string or any other custom string type,
// see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/imgui_stdlib.h file.
// Note: I copy theese two files directrly and put in Third/imgui.
#include "Third/imgui/imgui_stdlib.h"

namespace NoobRenderer
{
    namespace inspection
    {
        // Helper Function
        // This function is used to inspect a non-component material
        void InspectNonComponent(PBRMaterial::Ptr &pbrmat);

        inline static void Init()
        {
            ComponentUIMap::Register<component::NameComponent>();
            ComponentUIMap::Register<component::ActiveComponent>();
            ComponentUIMap::Register<component::Transform>();
            ComponentUIMap::Register<component::MeshData>();
            ComponentUIMap::Register<component::PointLight>();
            ComponentUIMap::Register<component::DirectionalLight>();
            ComponentUIMap::Register<component::AreaLight>();
            ComponentUIMap::Register<component::Camera>();
            ComponentUIMap::Register<component::IBLCubeMap>();
            ComponentUIMap::Register<component::ScreenSpaceReflection>();
        }

        template <>
        inline void InspectComponent<component::NameComponent>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            auto name = static_cast<component::NameComponent *>(typeAny);
            ImGui::Separator();
            ImGui::Button(IconFont::WithIcon(ICON_FA_VECTOR_SQUARE, "").c_str());
            ImGui::SameLine();
            if (ImGui::InputText("name##NameComponent", &name->name, ImGuiInputTextFlags_EnterReturnsTrue))
            {
            }
            // ImGui::Text("Component Name: %s", name->name.c_str());
        }

        template <>
        inline void InspectComponent<component::PointLight>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            auto pointlight = static_cast<component::PointLight *>(typeAny);
            bool visible = true;
            ImGui::Separator();
            if (ImGui::CollapsingHeader("PointLight Component", &visible, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("position##PointLight", glm::value_ptr<float>(pointlight->position), 0.1f);
                ImGui::DragFloat("linear##PointLight", &pointlight->linear, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("quadratic##PointLight", &pointlight->quadratic, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("constant##PointLight", &pointlight->constant, 0.05f, 0.0f, 100.0f);
                ImGui::ColorEdit3("color##PointLight", glm::value_ptr<float>(pointlight->color));
                ImGui::Checkbox("cast shadow##PointLight", &pointlight->shadow);
                if (pointlight->shadow)
                {
                    ImGui::DragFloat("near(light view)", &pointlight->near, 0.01f, 0.0f, 100.0f);
                    ImGui::DragFloat("far(light view)", &pointlight->far, 0.05f, 0.0f, 1000.0f);
                    ImGui::DragInt("ShadowMap Channel", &pointlight->shadow_channel, 1.0, 0, 2);
                }
            }
            if (visible == false)
            {
                entity->RemoveComponent<component::PointLight>();
            }
        }

        template <>
        inline void InspectComponent<component::AreaLight>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
        }

        template <>
        inline void InspectComponent<component::DirectionalLight>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto dirlight = static_cast<component::DirectionalLight *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("DirectionalLight Component", &visible, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("direction##DirectionalLight", glm::value_ptr<float>(dirlight->direction), 0.1f);
                ImGui::DragFloat("intensity##DirectionalLight", &dirlight->intensity, 0.05f, 0.f, 1.f);
                ImGui::ColorEdit3("color##DirectionalLight", glm::value_ptr<float>(dirlight->color));
                ImGui::Checkbox("cast shadow##DirectionalLight", &dirlight->shadow);
                if (dirlight->shadow)
                {
                    ImGui::DragFloat("bias factor", &dirlight->biasFactor, 0.001f, 0.f, 1.f);
                    ImGui::DragFloat("bias min-value", &dirlight->minBias, 0.0001f, 0.f, 1.f);
                    ImGui::DragFloat("sampling radius", &dirlight->shadowRadius, 0.5f, 0.f, 10.f);
                    ImGui::SeparatorText("Cascade Parameter");
                    for (int i = 0; i < dirlight->cascade_levels.size(); i++)
                    {
                        auto number = std::to_string(i);
                        ImGui::DragFloat((number + " - bias modifier").c_str(), &dirlight->biasModifier[i], 0.01f, 0.f, 10.f);
                        ImGui::DragFloat((number + " - palne split").c_str(), &dirlight->cascade_splits[i], 0.01f, 0.f, 1000.f);
                    }
                    ImGui::SeparatorText("Volumetric Light");
                    ImGui::DragFloat("scatter##DirectionalLight", &dirlight->vScatter, 0.001f, 0.f, 1.f);
                    ImGui::SliderInt("step##DirectionalLight", &dirlight->vStep, 1, 100);
                    ImGui::SliderFloat("intensity##DirectionalLight_1", &dirlight->vIntensity, 0.0f, 1.0f);
                }
            }
            if (!visible)
            {
                entity->RemoveComponent<component::DirectionalLight>();
            }
        }

        template <>
        inline void InspectComponent<component::Transform>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto transform = static_cast<component::Transform *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("Transform Component", &visible, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("transition##Transform", glm::value_ptr<float>(transform->position), 0.1f);
                ImGui::DragFloat3("rotate##Transform", glm::value_ptr<float>(transform->rotate), 0.1f);
                ImGui::DragFloat3("scale##Transform", glm::value_ptr<float>(transform->scale), 0.1f);
            }
            if (!visible)
            {
                entity->RemoveComponent<component::Transform>();
            }
        }

        template <>
        inline void InspectComponent<component::MeshData>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto meshdata = static_cast<component::MeshData *>(typeAny);
            auto mesh_ptr = meshdata->mesh;
            static DrawPrimitive primitive_list[] = {
                DrawPrimitive::Triangle,
                DrawPrimitive::TriangleStrip,
                DrawPrimitive::Line,
                DrawPrimitive::LineStrip,
                DrawPrimitive::LineStripAdjacency,
            };
            int combo_item_current = 0;
            ImGui::Separator();
            if (ImGui::CollapsingHeader("Meshs Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (mesh_ptr == nullptr)
                {
                    auto _width = ImGui::GetWindowWidth() * 0.95f;
                    ImGui::Button("Drop Meshs to Bind it.", ImVec2(_width, _width / 2));
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("This Feature will be added in furture version.");
                        ImGui::EndTooltip();
                    }
                }
                else
                {
                    ImGui::TextWrapped("Mesh Name: %s", meshdata->name.c_str());
                    static int combo_item_current = 0;
                    if (ImGui::BeginCombo("primitive", GetPrimitiveString(meshdata->primitive).data()))
                    {
                        for (int n = 0; n < 5; n++)
                        {
                            if (ImGui::Selectable(GetPrimitiveString(primitive_list[n]).data()))
                            {
                                meshdata->primitive = primitive_list[n];
                                meshdata->SyncMeshListPrimitive();
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }
                    auto mesh_cnt = 0;

                    ImGuiTreeNodeFlags NodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
                    auto &mat = mesh_ptr->GetMaterial();
                    auto type = mat->materialType;
                    ImGui::TextWrapped("Mesh Vertices: %i", mesh_ptr->GetVertexLength());
                    ImGui::TextWrapped("Mesh Indices: %i", mesh_ptr->GetIndiceLength());
                    if (ImGui::TreeNodeEx(IconFont::WithIcon(ICON_FA_LAYER_GROUP, "Material" + std::to_string(mesh_cnt++)).c_str(), NodeFlags))
                    {
                        switch (type)
                        {
                        case NoobRenderer::Material::Type::None:
                            ImGui::Text("Mat Type: None");
                            break;
                        case NoobRenderer::Material::Type::PBR:
                        {
                            auto pbrmat = std::dynamic_pointer_cast<NoobRenderer::PBRMaterial>(mat);
                            InspectNonComponent(pbrmat);
                            break;
                        }
                        }
                        ImGui::TreePop();
                    }
                }
                if (!visible)
                {
                    entity->RemoveComponent<component::MeshData>();
                }
            }
        }

        template <>
        inline void InspectComponent<component::Camera>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto camera = static_cast<component::Camera *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("position##Camera", glm::value_ptr<float>(camera->m_camera_position), 0.1f);
                ImGui::DragFloat3("moment##Camera", glm::value_ptr<float>(camera->m_camera_position_delta), 0.1f);
                ImGui::DragFloat3("up##Camera", glm::value_ptr<float>(camera->m_camera_up), 0.1f);
                ImGui::DragFloat3("lookat##Camera", glm::value_ptr<float>(camera->m_camera_look_at), 0.1f);
                ImGui::DragFloat3("direction##Camera", glm::value_ptr<float>(camera->m_camera_direction), 0.1f);
                ImGui::DragFloat("near##Camera", &camera->m_near_clip, 0.1f);
                ImGui::DragFloat("far##Camera", &camera->m_far_clip, 0.1f);
                ImGui::DragFloat("fov##Camera", &camera->m_field_of_view, 0.1f);
                ImGui::DragFloat("heading##Camera", &camera->m_camera_heading, 0.1f);
                ImGui::DragFloat("pitch##Camera", &camera->m_camera_pitch, 0.1f);
                ImGui::DragFloat("max heading##Camera", &camera->m_max_heading_rate, 0.1f);
                ImGui::DragFloat("max pitch##Camera", &camera->m_max_pitch_rate, 0.1f);
                ImGui::DragFloat("sensitivity##Camera", &camera->m_sensitivity, 0.1f);
                ImGui::DragFloat("speed##Camera", &camera->m_speed, 0.1f);
            }
            if (!visible)
            {
                entity->RemoveComponent<component::Camera>();
            }
        }

        template <>
        inline void InspectComponent<component::IBLCubeMap>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto cubeMap = static_cast<component::IBLCubeMap *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("IBLCubeMap Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                auto _width = ImGui::GetWindowWidth() * 0.95f;

                if (cubeMap->valid)
                {
                    if (ImGui::ImageButton((ImTextureID)(intptr_t)(cubeMap->RawHDRTexture->GetID()), ImVec2(_width / 2, _width / 2), ImVec2(0, 1), ImVec2(1, 0)))
                    {
                    }
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("CubeMap Preview##IBLCubeMap");
                        ImGui::EndTooltip();
                    }
                    ImGui::SameLine();
                    if (ImGui::ImageButton((ImTextureID)(intptr_t)(cubeMap->BRDFLookupTexture->GetID()), ImVec2(_width / 2, _width / 2), ImVec2(0, 1), ImVec2(1, 0)))
                    {
                    }
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::Text("BRDF-LUT##IBLCubeMap");
                        ImGui::EndTooltip();
                    }
                    if (ImGui::Button("Reset##IBLCubeMap", ImVec2(ImGui::GetWindowWidth() * 0.95f, 0.f)))
                    {
                        cubeMap->Reset();
                    }
                }
                else
                {
                    ImGui::DragInt("resolution##IBLCubeMap", &cubeMap->cubemap_size, 1.0f, 0, 4096);
                    ImGui::DragInt("conv-kernel size##IBLCubeMap", &cubeMap->conv_size, 1.0f, 0, 64);
                    ImGui::DragInt("Mipmap size##IBLCubeMap", &cubeMap->mipmap_size, 1.0f, 0, 256);
                    ImGui::DragInt("BRDF size##IBLCubeMap", &cubeMap->brdf_size, 1.0f, 0, 1024);

                    if (ImGui::Button("Load IBL CubeMap##IBLCubeMap", ImVec2(ImGui::GetWindowWidth() * 0.95f, 0.f)))
                    {
                        IGFD::FileDialogConfig config;
                        config.path = ".";
                        config.sidePaneWidth = 400;
                        ImGuiFileDialog::Instance()->OpenDialog("Load CubeMap From File", "Choose File", ".hdr", config);
                    }
                }
            }
            if (!visible)
            {
                entity->RemoveComponent<component::IBLCubeMap>();
            }
        }

        template <>
        inline void InspectComponent<component::ScreenSpaceReflection>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto ssr = static_cast<component::ScreenSpaceReflection *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("ScreenSpaceReflection Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("Enable", &ssr->enable);
            }
            if (!visible)
            {
                entity->RemoveComponent<component::ScreenSpaceReflection>();
            }
        }

        template <>
        inline void InspectComponent<component::VoxelGlobalIllumination>(NoobRenderer::Entity::Ptr &entity, void *typeAny)
        {
            bool visible = true;
            auto vxgi = static_cast<component::VoxelGlobalIllumination *>(typeAny);
            ImGui::Separator();
            if (ImGui::CollapsingHeader("VoxelGlobalIllumination Component", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SliderInt("Voxel Size##VoxelGlobalIllumination", &vxgi->voxel_size, 1, 500);
            }
            if (!visible)
            {
                entity->RemoveComponent<component::ScreenSpaceReflection>();
            }
        }
    }
}