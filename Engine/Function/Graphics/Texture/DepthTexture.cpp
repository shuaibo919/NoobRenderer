#include "Engine/Function/Graphics/Texture/DepthTexture.h"
using namespace NoobRenderer;

DepthTexture::DepthTexture(int width, int height)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer,
                  gtype::TexType::Tex2D, gtype::Format::DEPTH_COMPONENT,
                  gtype::Format::DEPTH_COMPONENT, gtype::DataType::FLOAT)
{
    this->m_params.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Nearest);
    this->m_params.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Nearest);
    this->m_params.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampBorder);
    this->m_params.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampBorder);
    SettingTexture();
}

void DepthTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->m_params.Apply(target);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    this->SetParameter(gtype::TexParaType::BorderColor, borderColor);
    this->TexImage2D(target, 0, 0, NULL);
}

DepthTextureArray::DepthTextureArray(int width, int height, int layres)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer,
                  gtype::TexType::Tex2DArray, gtype::Format::DEPTH_COMPONENT,
                  gtype::Format::DEPTH_COMPONENT, gtype::DataType::FLOAT),
      m_layers(layres)
{

    SettingTexture();
}

void DepthTextureArray::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage3D(target, 0, 0, m_layers, NULL);
    this->m_params.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Nearest);
    this->m_params.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Nearest);
    this->m_params.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampBorder);
    this->m_params.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampBorder);
    this->m_params.Apply(target);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    this->SetParameter(gtype::TexParaType::BorderColor, borderColor);
}
