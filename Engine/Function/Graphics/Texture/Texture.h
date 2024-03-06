#pragma once
#include "Engine/Function/Graphics/Texture/TextureBase.h"

namespace NoobRenderer
{

    class OrdinaryTexture : public TextureBase
    {
    public:
        using Ptr = std::shared_ptr<OrdinaryTexture>;

    protected:
        std::string m_path;

    public:
        OrdinaryTexture(const std::string &path, const Texture::Type &type = Texture::Type::None,
                        gtype::DataType datatype = gtype::DataType::UNSIGNED_BYTE);
        OrdinaryTexture(const std::string &path, int specify_channel, const Texture::Type &type = Texture::Type::None,
                        gtype::DataType datatype = gtype::DataType::UNSIGNED_BYTE);
        OrdinaryTexture(const OrdinaryTexture &texture);
        OrdinaryTexture(OrdinaryTexture &&texture) noexcept;
        void SetDefaultImageTextureParams();

    protected:
        void SettingTextureFromFile(const std::string &path);
        void SettingTextureFromFile(const std::string &path, int channel);
        void SettingTexture() override;

    public:
        inline void SetPath(const std::string &path) { this->m_path = path; }
        inline const std::string &GetPath() { return m_path; }
    };

}
