#include "Engine/Function/Graphics/Texture/ColorTexture.h"
using namespace NoobRenderer;
ColorTexture::ColorTexture(int width, int height, gtype::Format tex_format,
                           gtype::Format tex_internal_format, gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2D, tex_format, tex_internal_format, datatype)
{
    this->m_params.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Nearest);
    this->m_params.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Nearest);
    SettingTexture();
}
ColorTexture::ColorTexture(int width, int height, TextureParameter params,
                           gtype::Format tex_format, gtype::Format tex_internal_format, gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2D, tex_format, tex_internal_format, datatype)
{
    this->m_params = params;
    SettingTexture();
}
ColorTexture::ColorTexture(int width, int height, TextureParameter params, void *buf,
                           gtype::Format tex_format, gtype::Format tex_internal_format, gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, gtype::TexType::Tex2D, tex_format, tex_internal_format, datatype)
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->m_params = params;
    this->Bind();
    this->TexImage2D(target, 0, 0, buf);
    this->m_params.Apply(target);
}
void ColorTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, nullptr);
    this->m_params.Apply(target);
}