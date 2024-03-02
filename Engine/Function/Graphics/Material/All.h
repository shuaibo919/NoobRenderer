#pragma
#include "Engine/Function/Graphics/Material/Material.h"
#include "Engine/Function/Graphics/Material/PBRMaterial.h"
namespace NoobRenderer
{
    Material::Ptr GetMaterialByCopyCtor(Material::Ptr material);
    Material::Ptr GetMaterialByCopyCtor(nlohmann::json material);
}