#include "Engine/Function/Graphics/Texture/ColorTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace NoobRenderer;
Texture2D::Texture2D(int width, int height, Texture::Format tex_format, Texture::Format tex_internal_format, Texture::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex2D, tex_format, tex_internal_format, datatype),
      m_path("")
{
    SettingTexture();
}
Texture2D::Texture2D(int width, int height, void *buf, Texture::Format tex_format, Texture::Format tex_internal_format, Texture::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex2D, tex_format, tex_internal_format, datatype),
      m_path("")
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, buf);
}
Texture2D::Texture2D(const std::string &path, const Texture::Type &type, Texture::DataType datatype, bool load_filp)
    : TextureBase(0, 0, type, Texture::BaseType::Tex2D, Texture::Format::NONE, Texture::Format::NONE, datatype),
      m_path(path)
{
    SettingTextureFromFile(m_path, load_filp);
}
Texture2D::Texture2D(Texture::Format tex_format, Texture::Format tex_internal_format,
                     Texture::DataType datatype, const std::string &path, bool load_filp)
    : TextureBase(0, 0, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex2D, tex_format, tex_internal_format, datatype),
      m_path(path)
{
    stbi_set_flip_vertically_on_load(load_filp);
    int nrComponents;
    void *data = stbi_loadf(path.c_str(), &m_width, &m_height, &nrComponents, 0);
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, data);
    glGenerateMipmap(target);
    if (data)
        stbi_image_free(data);
}
Texture2D::Texture2D(const std::string &path, int specify_channel, const Texture::Type &type, Texture::DataType datatype, bool load_filp)
    : TextureBase(0, 0, type, Texture::BaseType::Tex2D, Texture::Format::NONE, Texture::Format::NONE, datatype),
      m_path(path)
{
    if (specify_channel < 0)
    {
        SettingTextureFromFile(m_path, load_filp);
    }
    else
    {
        SettingTextureFromFile(m_path, specify_channel);
    }
}
Texture2D::Texture2D(const Texture2D &texture)
    : m_path(texture.m_path), TextureBase(texture)
{
}

Texture2D::Texture2D(Texture2D &&texture) noexcept
    : m_path(std::move(texture.m_path)), TextureBase(std::move(texture))
{
}
void Texture2D::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, nullptr);
}
void Texture2D::SettingTextureFromFile(const std::string &path, bool load_filp)
{
    stbi_set_flip_vertically_on_load(load_filp);
    int nrComponents;
    void *data = stbi_load(path.c_str(), &m_width, &m_height, &nrComponents, 0);
    m_format = Texture::Format::RGBA;
    if (nrComponents == 1)
        m_format = Texture::Format::RED;
    else if (nrComponents == 3)
        m_format = Texture::Format::RGB;
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
void Texture2D::SettingTextureFromFile(const std::string &path, int channel, bool load_filp)
{
    stbi_set_flip_vertically_on_load(load_filp);
    int nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &m_width, &m_height, &nrComponents, 0);

    if (data)
    {
        unsigned char *data_channel = new unsigned char[m_width * m_height];
        for (int i = 0; i < m_width * m_height; ++i)
        {
            data_channel[i] = data[i * nrComponents + channel];
        }
        m_format = Texture::Format::RED;
        m_internalformat = Texture::Format::RGB;

        GLenum base_type = static_cast<GLenum>(m_basetype);
        this->Bind();
        this->TexImage2D(base_type, 0, 0, data_channel);
        this->GenerateMipmap();
        delete[] data_channel;
        stbi_image_free(data);
    }
}
void Texture2D::SetDefaultImageTextureParams()
{
    this->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    this->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->SetParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    this->SetParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
}
Texture2DArray::Texture2DArray(int width, int height, int layers, Texture::Format format,
                               Texture::Format internal_format, Texture::DataType datatype)
    : m_layers(layers), TextureBase(width, height, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex2DArray, format, internal_format, datatype)

{
    SettingTexture();
}
void Texture2DArray::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage3D(target, 0, 0, m_layers, NULL);
}
Texture3D::Texture3D(int width, int height, int depth, Texture::Format format,
                     Texture::Format internal_format, Texture::DataType datatype)
    : m_depth(depth), TextureBase(width, height, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex3D, format, internal_format, datatype)
{
    SettingTexture();
}
void Texture3D::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage3D(target, 0, 0, m_depth, NULL);
}
