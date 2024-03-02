#include "Engine/Function/Graphics/Texture/HDRBufferTexture.h"
#include "stb_image.h"
using namespace NoobRenderer;
HDRBufferTexture::HDRBufferTexture(const std::string &path)
    : TextureBase(0, 0, Texture::Type::TextureHDR, gtype::TexType::Tex2D, gtype::Format::RGB, gtype::Format::RGB16F, gtype::DataType::FLOAT)

{
    this->m_params.Add(gtype::TexParaType::WrapS, gtype::TexPara::ClampEdge);
    this->m_params.Add(gtype::TexParaType::WrapT, gtype::TexPara::ClampEdge);
    this->m_params.Add(gtype::TexParaType::MinFilter, gtype::TexPara::Linear);
    this->m_params.Add(gtype::TexParaType::MagFilter, gtype::TexPara::Linear);
    SettingTextureFromFile(path);
}
void HDRBufferTexture::SettingTexture()
{
}

void HDRBufferTexture::SettingTextureFromFile(const std::string &path)
{
    stbi_set_flip_vertically_on_load(true);
    int nrComponents;
    void *data = stbi_loadf(path.c_str(), &m_width, &m_height, &nrComponents, 0);
    GLenum target = static_cast<GLenum>(m_basetype);
    this->Bind();
    this->m_params.Apply(target);
    this->TexImage2D(target, 0, 0, data);
    glGenerateMipmap(target);
    if (data)
        stbi_image_free(data);
}