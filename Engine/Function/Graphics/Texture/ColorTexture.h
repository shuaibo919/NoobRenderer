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
        Texture2D(int width, int height, void *buf,
                  gtype::Format tex_format = gtype::Format::RGB,
                  gtype::Format tex_internal_format = gtype::Format::RGB32F,
                  gtype::DataType datatype = gtype::DataType::FLOAT);
        Texture2D() = delete;

    private:
        void SettingTexture() override;
    };
    class Texture2DArray : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<Texture2DArray>;
        Texture2DArray(int width, int height, int layers,
                       gtype::Format format = gtype::Format::RGB,
                       gtype::Format internal_format = gtype::Format::RGB32F,
                       gtype::DataType datatype = gtype::DataType::FLOAT);
        int GetLayers() const { return m_layers; }

    private:
        int m_layers;
        void SettingTexture() override;
    };

    class Texture3D : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<Texture3D>;
        Texture3D(int width, int height, int depth,
                  gtype::Format format = gtype::Format::RGB,
                  gtype::Format internal_format = gtype::Format::RGB32F,
                  gtype::DataType datatype = gtype::DataType::FLOAT);
        Texture3D() = delete;
        int GetDepth() const { return m_depth; }

    private:
        int m_depth;
        void SettingTexture() override;
    };
}