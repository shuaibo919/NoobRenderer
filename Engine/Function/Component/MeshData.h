#pragma once
#include "Engine/Function/Graphics/MeshManager.h"
#include "Engine/Core/Serialization.h"
namespace NoobRenderer
{
    namespace component
    {
        struct MeshData
        {
            std::string name;
            MeshIndex index;
            Mesh::Ptr mesh{nullptr};
            DrawPrimitive primitive{DrawPrimitive::Triangle};
            MeshData(std::string_view _name, MeshIndex _id = 0) : name(_name), index(_id) { RetrieveMeshFromManager(); }
            MeshData() : mesh(nullptr), index(0) {}
            inline void RetrieveMeshFromManager()
            {
                auto mesh_ptr = MeshManager::Instance().RetrieveMeshShared(name, index);
                if (mesh_ptr == nullptr)
                {
                    std::cout << "MeshData::RetrieveMeshFromManager() Failed!" << std::endl;
                    return;
                }
                mesh = mesh_ptr;
                this->primitive = mesh->GetPrimitive();
            }

            inline void SyncMeshListPrimitive()
            {
                mesh->SetPrimitive(primitive);
            }

            inline void AssignDefaultMaterial()
            {
                PBRMaterial::Ptr mat = std::make_shared<PBRMaterial>();
                mat->albedo = std::make_shared<MaterialTexture>(glm::vec3(0.5f, 0.5f, 0.5f));
                mat->metallic = std::make_shared<MaterialTexture>(0.01f);
                mat->roughness = std::make_shared<MaterialTexture>(0.05f);
                mat->emission = std::make_shared<MaterialTexture>(glm::vec3(0.0f));
                mesh->SetMaterial(mat);
            }

            inline friend void to_json(nlohmann::json &nlohmann_json_j, const MeshData &nlohmann_json_t)
            {
                nlohmann_json_j["mesh"] = nlohmann_json_t.name;
                nlohmann_json_j["mesh_id"] = static_cast<int>(nlohmann_json_t.index);
                nlohmann_json_j["primitive"] = static_cast<int>(nlohmann_json_t.primitive);
                nlohmann_json_j["material"] = nlohmann::json::array();
                auto &mat = nlohmann_json_t.mesh->GetMaterial();
                if (mat == nullptr)
                    nlohmann_json_j["material"] = "null";
                else
                    nlohmann_json_j["material"] = mat->ToMaterialFile();
            }
            inline friend void from_json(const nlohmann::json &nlohmann_json_j, MeshData &nlohmann_json_t)
            {
                nlohmann_json_j.at("mesh").get_to(nlohmann_json_t.name);
                nlohmann_json_j.at("mesh_id").get_to(nlohmann_json_t.index);
                nlohmann_json_j.at("primitive").get_to(nlohmann_json_t.primitive);
                nlohmann_json_t.RetrieveMeshFromManager(); // Note!

                if (nlohmann_json_j["material"] == "null")
                    return;
                auto mat_ptr = GetMaterialByCopyCtor(nlohmann_json_j["material"]);
                nlohmann_json_t.mesh->SetMaterial(mat_ptr);
            }
            DESERILIZING_TO_ENTITY(MeshData)
            SERIALIZATION_FROM_TYPE_ANY(MeshData)
        };

    } // namespace component
};
