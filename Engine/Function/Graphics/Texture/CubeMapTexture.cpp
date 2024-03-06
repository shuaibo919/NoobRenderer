#include "Engine/Function/Graphics/Texture/CubeMapTexture.h"
using namespace NoobRenderer;
CubeMapTexture::CubeMapTexture(unsigned int width, unsigned int height, gtype::Format format, gtype::Format interformat,
                               gtype::DataType datatype)
    : TextureBase(width, height, Texture::Type::TextureCubeMap, gtype::TexType::TexCube, format, interformat, datatype)
{
    SettingTexture();
}
void CubeMapTexture::SettingTexture()
{
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        this->TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, nullptr);
    }
}