#include "Engine/Function/Graphics/Texture/LookUpTexture.h"
using namespace NoobRenderer;
LookUpTexture::LookUpTexture(unsigned int width, unsigned int height, gtype::Format format, gtype::Format internalformat, gtype::DataType datatype,
                             const Texture::Type &type)
    : TextureBase(width, height, type, gtype::TexType::Tex2D, format, internalformat, datatype)
{
    this->m_params.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampEdge);
    this->m_params.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampEdge);
    this->m_params.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Linear);
    this->m_params.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Linear);
    SettingTexture();
}
void LookUpTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2D(target, 0, 0, NULL);
    this->m_params.Apply(target);
}
