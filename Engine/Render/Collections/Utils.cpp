#include "Engine/Render/Collections/Utils.h"
namespace NoobRenderer
{
    namespace render
    {
        namespace utils
        {
            bool SetMaterialUniform(MaterialTexture::Ptr &mtex, ShaderProgram::Ptr &shader, const std::string &name, MatUniValue op)
            {
                if (mtex == nullptr)
                    return false;
                if (mtex->texture2d != nullptr)
                {
                    mtex->texture2d->Activate();
                    shader->SetUniform("materialMap." + std::string(mtex->texture2d->GetTypeString()), mtex->texture2d->GetTempSlot());
                    return true;
                }

                switch (op)
                {
                case MatUniValue::Float:
                    shader->SetUniform("material." + name, mtex->factor.r);
                    break;
                case MatUniValue::Vec3:
                    shader->SetUniform("material." + name, mtex->factor);
                    break;
                default:
                    break;
                }

                return false;
            }
            bool SetMaterialUniform(MaterialTexture::Ptr &mtex, ShaderProgram::Ptr &shader)
            {
                if (mtex == nullptr)
                    return false;
                if (mtex->texture2d != nullptr)
                {
                    mtex->texture2d->Activate();
                    shader->SetUniform("materialMap." + std::string(mtex->texture2d->GetTypeString()), mtex->texture2d->GetTempSlot());
                    return true;
                }
                return false;
            }
        }
    }
}
