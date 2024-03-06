#include "Engine/Function/Graphics/Texture/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace NoobRenderer;
OrdinaryTexture::OrdinaryTexture(const std::string &path, const Texture::Type &type, gtype::DataType datatype)
    : TextureBase(0, 0, type, gtype::TexType::Tex2D, gtype::Format::NONE, gtype::Format::NONE, datatype),
      m_path(path)
{
    SettingTextureFromFile(m_path);
}
OrdinaryTexture::OrdinaryTexture(const std::string &path, int specify_channel, const Texture::Type &type, gtype::DataType datatype)
    : TextureBase(0, 0, type, gtype::TexType::Tex2D, gtype::Format::NONE, gtype::Format::NONE, datatype),
      m_path(path)
{
    if (specify_channel < 0)
    {
        SettingTextureFromFile(m_path);
    }
    else
    {
        SettingTextureFromFile(m_path, specify_channel);
    }
}
OrdinaryTexture::OrdinaryTexture(const OrdinaryTexture &texture)
    : m_path(texture.m_path), TextureBase(texture)
{
}

OrdinaryTexture::OrdinaryTexture(OrdinaryTexture &&texture) noexcept
    : m_path(std::move(texture.m_path)), TextureBase(std::move(texture))
{
}

void OrdinaryTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, NULL);
}
void OrdinaryTexture::SettingTextureFromFile(const std::string &path)
{
    stbi_set_flip_vertically_on_load(false);
    int nrComponents;
    void *data = stbi_load(path.c_str(), &m_width, &m_height, &nrComponents, 0);
    m_format = gtype::Format::RGBA;
    if (nrComponents == 1)
        m_format = gtype::Format::RED;
    else if (nrComponents == 3)
        m_format = gtype::Format::RGB;
    m_internalformat = m_format;

    GLenum base_type = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(base_type, 0, 0, data);
    if (data)
    {
        this->GenerateMipmap();
        stbi_image_free(data);
    }
}
void OrdinaryTexture::SettingTextureFromFile(const std::string &path, int channel)
{
    stbi_set_flip_vertically_on_load(false);
    int nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &m_width, &m_height, &nrComponents, 0);

    if (data)
    {
        unsigned char *data_channel = new unsigned char[m_width * m_height];
        for (int i = 0; i < m_width * m_height; ++i)
        {
            data_channel[i] = data[i * nrComponents + channel];
        }
        m_format = gtype::Format::RED;
        m_internalformat = gtype::Format::RGB;

        GLenum base_type = static_cast<GLenum>(m_basetype);
        GLenum base_type = static_cast<GLenum>(m_basetype);
        this->Bind();
        this->TexImage2D(base_type, 0, 0, data_channel);
        this->GenerateMipmap();
        delete[] data_channel;
        stbi_image_free(data);
    }
}
