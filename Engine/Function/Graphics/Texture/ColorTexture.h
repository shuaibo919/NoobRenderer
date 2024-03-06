#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{
    class Texture2D : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<Texture2D>;
        // Default MinFilter & MagFilter: Nearest
        Texture2D(int width, int height,
                  gtype::Format tex_format = gtype::Format::RGB,
                  gtype::Format tex_internal_format = gtype::Format::RGB32F,
                  gtype::DataType datatype = gtype::DataType::FLOAT);
        Texture2D(int width, int height, TextureParameter params,
                  gtype::Format tex_format = gtype::Format::RGB,
                  gtype::Format tex_internal_format = gtype::Format::RGB32F,
                  gtype::DataType datatype = gtype::DataType::FLOAT);
        Texture2D(int width, int height, TextureParameter params, void *buf,
                  gtype::Format tex_format = gtype::Format::RGB,
                  gtype::Format tex_internal_format = gtype::Format::RGB32F,
                  gtype::DataType datatype = gtype::DataType::FLOAT);
        Texture2D() = delete;

    private:
        void SettingTexture() override;
    };
}