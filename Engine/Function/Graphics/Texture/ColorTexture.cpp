#include "Engine/Function/Graphics/Texture/ColorTexture.h"
using namespace NoobRenderer;
Texture2D::Texture2D(int width, int height, gtype::Format tex_format, gtype::Format tex_internal_format, gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2D, tex_format, tex_internal_format, datatype)
{
    SettingTexture();
}
Texture2D::Texture2D(int width, int height, void *buf, gtype::Format tex_format, gtype::Format tex_internal_format, gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2D, tex_format, tex_internal_format, datatype)
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, buf);
}
void Texture2D::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, nullptr);
}
Texture2DArray::Texture2DArray(int width, int height, int layers, gtype::Format format,
                               gtype::Format internal_format, gtype::DataType datatype)
    : m_layers(layers), TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2DArray, format, internal_format, datatype)

{
    SettingTexture();
}
void Texture2DArray::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage3D(target, 0, 0, m_layers, NULL);
}
Texture3D::Texture3D(int width, int height, int depth, gtype::Format format,
                     gtype::Format internal_format, gtype::DataType datatype)
    : m_depth(depth), TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex3D, format, internal_format, datatype)
{
    SettingTexture();
}
void Texture3D::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage3D(target, 0, 0, m_depth, NULL);
}