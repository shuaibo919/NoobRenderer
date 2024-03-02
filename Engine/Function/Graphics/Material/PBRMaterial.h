#pragma once
#include "Engine/Function/Graphics/Material/Material.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
namespace NoobRenderer
{
    struct PBRMaterial : public Material
    {
        std::string name;
        using Ptr = std::shared_ptr<PBRMaterial>;
        PBRMaterial(Type _materialType = Material::Type::PBR) : Material(_materialType){};
        PBRMaterial(const PBRMaterial &material) : Material(material)
        {
            albedo = std::make_shared<MaterialTexture>(material.albedo);
            metallic = std::make_shared<MaterialTexture>(material.metallic);
            roughness = std::make_shared<MaterialTexture>(material.roughness);
            emission = std::make_shared<MaterialTexture>(material.emission);
        }
        PBRMaterial(PBRMaterial &&material) noexcept : Material(std::move(material)), albedo(std::move(material).albedo), emission(std::move(material).emission),
                                                       metallic(std::move(material).metallic), roughness(std::move(material).roughness) {}
        ~PBRMaterial() = default;

        MaterialTexture::Ptr albedo{nullptr}, emission{nullptr}, metallic{nullptr}, roughness{nullptr};
        nlohmann::json ToMaterialFile() override
        {
            nlohmann::json j;
            j["type"] = static_cast<int>(materialType);
            j["albedo"] = (albedo != nullptr) ? albedo->ToMaterialFile() : "null";
            j["emission"] = (emission != nullptr) ? emission->ToMaterialFile() : "null";
            j["metallic"] = (metallic != nullptr) ? metallic->ToMaterialFile() : "null";
            j["roughness"] = (roughness != nullptr) ? roughness->ToMaterialFile() : "null";
            j["height"] = (height != nullptr) ? height->ToMaterialFile() : "null";
            j["normal"] = (normal != nullptr) ? normal->ToMaterialFile() : "null";
            return j;
        }
        inline static PBRMaterial::Ptr CreateFromMaterialFile(nlohmann::json j)
        {
            auto _ptr = std::make_shared<PBRMaterial>();
            _ptr->materialType = static_cast<Material::Type>(j["type"]);
            _ptr->albedo = j["albedo"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["albedo"]);
            _ptr->emission = j["emission"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["emission"]);
            _ptr->metallic = j["metallic"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["metallic"]);
            _ptr->roughness = j["roughness"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["roughness"]);
            _ptr->height = j["height"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["height"]);
            _ptr->normal = j["normal"] == "null" ? nullptr : MaterialTexture::CreateFromMaterialFile(j["normal"]);
            return _ptr;
        }
    };
}