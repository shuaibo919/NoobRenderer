#include "Application/Inspector/Inspector.h"
#include "Application/Common.h"

namespace NoobRenderer
{
    namespace inspection
    {
        void InspectMaterialTexture(std::string_view name, MaterialTexture::Ptr &mattex, Texture::Type type, std::function<void(std::string_view, MaterialTexture::Ptr &)> &assistFunc)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
            ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);

            if (ImGui::TreeNodeEx(IconFont::WithIcon(ICON_FA_BORDER_ALL, std::string(name)).c_str(), flags))
            {
                assistFunc(name, mattex);
                if (mattex == nullptr)
                {
                    ImGui::Text("Texture Map: Null");
                }
                else if (mattex->texture2d == nullptr)
                {
                    if (mattex->factor.b < 0.0f)
                        ImGui::DragFloat("texture", glm::value_ptr<float>(mattex->factor), 0.01f, 0.0f, 1.0f);

                    else
                        ImGui::DragFloat3("texture", glm::value_ptr<float>(mattex->factor), 0.01f, 0.0f, 1.0f);
                    ImGui::Text("Texture Map: Null");
                }
                else
                {
                    auto _width = ImGui::GetWindowWidth() * 0.45f;
                    ImGui::Image((ImTextureID)(intptr_t)(mattex->texture2d->GetID()), ImVec2(_width, _width), ImVec2(0, 1), ImVec2(1, 0));
                    auto name = EngineUtility::GetFileName(mattex->texture2d->GetPath());
                    // ImGui::Text("Texture Map: %s", name.c_str());
                    // ImGui::Image(tex->GetTextureID(), ImVec2(tex->GetWidth(), tex->GetHeight()));
                }
                ImGui::TreePop();
            }
            auto getfile = common::ShowFileDiaglogue(std::string(name));
            if (getfile.first != "")
            {
                auto tex_2d = std::make_shared<Texture2D>(getfile.first, type);
                mattex = std::make_shared<MaterialTexture>(tex_2d);
            }
            ImGui::PopStyleVar();
        }
        void InspectNonComponent(PBRMaterial::Ptr &pbrmat)
        {
            std::function<void(std::string_view, MaterialTexture::Ptr &)> assistFunc = [](std::string_view name, MaterialTexture::Ptr &mattex)
            {
                auto _width = ImGui::GetWindowWidth() * 0.275f;
                if (ImGui::Button("Change", ImVec2(_width, 0)))
                {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.sidePaneWidth = 400;
                    const char *filters = "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}";
                    ImGuiFileDialog::Instance()->OpenDialog(std::string(name), "Choose File", filters, config);
                }
                ImGui::SameLine();
                if (ImGui::Button("Reset", ImVec2(_width, 0)))
                {
                    if (mattex != nullptr)
                        mattex.reset();
                }
                ImGui::SameLine();
                if (ImGui::Button("Create", ImVec2(_width, 0)))
                {
                    ImGui::OpenPopup("Create Popup");
                }
                if (ImGui::BeginPopup("Create Popup"))
                {
                    ImGui::SeparatorText("Choose a MaterialTexture");
                    if (ImGui::Selectable("Float"))
                    {
                        mattex.reset();
                        mattex = std::make_shared<MaterialTexture>(0.5f);
                    }
                    if (ImGui::Selectable("Vector3"))
                    {
                        mattex.reset();
                        mattex = std::make_shared<MaterialTexture>(glm::vec3(0.5f));
                    }
                    ImGui::EndPopup();
                }
            };
            InspectMaterialTexture("Albedo", pbrmat->albedo, Texture::Type::TextureAlbedo, assistFunc);
            InspectMaterialTexture("Emisssion", pbrmat->emission, Texture::Type::TextureEmission, assistFunc);
            InspectMaterialTexture("Metallic", pbrmat->metallic, Texture::Type::TextureMetallic, assistFunc);
            InspectMaterialTexture("Roughnesss", pbrmat->roughness, Texture::Type::TextureRoughness, assistFunc);
            InspectMaterialTexture("Normal", pbrmat->normal, Texture::Type::TextureNormal, assistFunc);
            InspectMaterialTexture("Height", pbrmat->height, Texture::Type::TextureDepth, assistFunc);
        }
    }
}