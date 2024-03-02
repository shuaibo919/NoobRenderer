#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{
    class ColorTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<ColorTexture>;
        // Default MinFilter & MagFilter: Nearest
        ColorTexture(int width, int height,
                     gtype::Format tex_format = gtype::Format::RGB,
                     gtype::Format tex_internal_format = gtype::Format::RGB32F,
                     gtype::DataType datatype = gtype::DataType::FLOAT);
        ColorTexture(int width, int height, TextureParameter params,
                     gtype::Format tex_format = gtype::Format::RGB,
                     gtype::Format tex_internal_format = gtype::Format::RGB32F,
                     gtype::DataType datatype = gtype::DataType::FLOAT);
        ColorTexture(int width, int height, TextureParameter params, void *buf,
                     gtype::Format tex_format = gtype::Format::RGB,
                     gtype::Format tex_internal_format = gtype::Format::RGB32F,
                     gtype::DataType datatype = gtype::DataType::FLOAT);
        ColorTexture() = delete;

    private:
        void SettingTexture() override;
    };
}