#include "Engine/Function/Graphics/Material/All.h"
#include "iostream"
namespace NoobRenderer
{
    /// @brief 复制材质, 匹配各自的复制构造函数
    /// @param material
    /// @return Material::Ptr
    Material::Ptr GetMaterialByCopyCtor(Material::Ptr material)
    {
        if (material == nullptr)
            return nullptr;
        auto type = material->materialType;
        switch (type)
        {
        case Material::Type::None:
            break;
        case Material::Type::PBR:
            return std::make_shared<PBRMaterial>(*(PBRMaterial *)material.get());
            break;

        default:
            break;
        }
        return nullptr;
    }

    Material::Ptr GetMaterialByCopyCtor(nlohmann::json material)
    {
        Material::Type type = static_cast<Material::Type>(material["type"]);
        switch (type)
        {
        case Material::Type::None:
            break;
        case Material::Type::PBR:
            return PBRMaterial::CreateFromMaterialFile(material);
            break;

        default:
            break;
        }
        return nullptr;
    }

}