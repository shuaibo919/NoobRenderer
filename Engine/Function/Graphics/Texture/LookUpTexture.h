#pragma once

#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{

    class LookUpTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<LookUpTexture>;
        LookUpTexture(unsigned int width, unsigned int height, gtype::Format format, gtype::Format internalformat, gtype::DataType datatype,
                      const Texture::Type &type = Texture::Type::None);
        LookUpTexture() = delete;

    private:
        void SettingTexture() override;
    };
}