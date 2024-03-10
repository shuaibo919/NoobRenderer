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
                  Texture::Format tex_format = Texture::Format::RGB,
                  Texture::Format tex_internal_format = Texture::Format::RGB32F,
                  Texture::DataType datatype = Texture::DataType::FLOAT);
        Texture2D(int width, int height, void *buf,
                  Texture::Format tex_format = Texture::Format::RGB,
                  Texture::Format tex_internal_format = Texture::Format::RGB32F,
                  Texture::DataType datatype = Texture::DataType::FLOAT);
        Texture2D(const std::string &path, const Texture::Type &type = Texture::Type::None,
                  Texture::DataType datatype = Texture::DataType::UNSIGNED_BYTE, bool load_filp = false);
        Texture2D(const std::string &path, int specify_channel, const Texture::Type &type = Texture::Type::None,
                  Texture::DataType datatype = Texture::DataType::UNSIGNED_BYTE, bool load_filp = false);
        Texture2D(Texture::Format tex_format, Texture::Format tex_internal_format,
                  Texture::DataType datatype, const std::string &path, bool load_filp = false);
        Texture2D(const Texture2D &texture);
        Texture2D(Texture2D &&texture) noexcept;
        Texture2D() = delete;
        void SetDefaultImageTextureParams();

    public:
        void SetPath(const std::string &path) { this->m_path = path; }
        const std::string &GetPath() { return m_path; }

    private:
        std::string m_path;
        void SettingTexture() override;
        void SettingTextureFromFile(const std::string &path, bool load_filp = false);
        void SettingTextureFromFile(const std::string &path, int channel, bool load_filp = false);
    };
    class Texture2DArray : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<Texture2DArray>;
        Texture2DArray(int width, int height, int layers,
                       Texture::Format format = Texture::Format::RGB,
                       Texture::Format internal_format = Texture::Format::RGB32F,
                       Texture::DataType datatype = Texture::DataType::FLOAT);
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
                  Texture::Format format = Texture::Format::RGB,
                  Texture::Format internal_format = Texture::Format::RGB32F,
                  Texture::DataType datatype = Texture::DataType::FLOAT);
        Texture3D() = delete;
        int GetDepth() const { return m_depth; }

    private:
        int m_depth;
        void SettingTexture() override;
    };
}