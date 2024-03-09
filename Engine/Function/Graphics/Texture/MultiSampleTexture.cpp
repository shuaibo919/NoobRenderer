#include "Engine/Function/Graphics/Texture/MultiSampleTexture.h"
using namespace NoobRenderer;
MultiSampleTexture::MultiSampleTexture(unsigned int width, unsigned int height, Texture::Format internalformat, int samples,
                                       const Texture::Type &type)
    : TextureBase(width, height, Texture::Type::TextureColorBuffer, Texture::BaseType::Tex2DMultisample, internalformat, internalformat, Texture::DataType::UNSIGNED_BYTE)
{
    SettingTexture();
}
void MultiSampleTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->TexImage2DMultisample(target, 4, GL_TRUE);
}
