#include "Engine/Function/Graphics/Texture/TextureBase.h"
using namespace NoobRenderer;

TextureParameter::TextureParameter() {}
void TextureParameter::Add(gtype::TexParaType type, gtype::TexPara para)
{
    m_paras.emplace_back(std::make_pair(type, para));
}
void TextureParameter::Apply(GLenum target)
{
    for (auto &para : m_paras)
    {
        glTexParameteri(target, static_cast<GLenum>(para.first), static_cast<GLint>(para.second));
    }
}

TextureBase::TextureBase(int width, int height, Texture::Type type, gtype::TexType basetype,
                         gtype::Format format, gtype::Format internalformat, gtype::DataType datatype)
    : m_width(width), m_height(height), m_type(type), m_basetype(basetype),
      m_format(format), m_internalformat(internalformat), m_datatype(datatype)
{
    InitTextureUnits();
    glGenTextures(1, &m_id);
}

TextureBase::TextureBase(const TextureBase &texture)
    : m_id(texture.m_id), m_width(texture.m_width),
      m_height(texture.m_height), m_type(texture.m_type), m_basetype(texture.m_basetype),
      m_format(texture.m_format),
      m_internalformat(texture.m_internalformat), m_datatype(texture.m_datatype)
{
    m_tmp_slot = 0;
}
TextureBase::TextureBase(TextureBase &&texture) noexcept : m_id(std::move(texture.m_id)), m_width(std::move(texture.m_width)),
                                                           m_height(std::move(texture.m_height)), m_type(std::move(texture.m_type)),
                                                           m_basetype(std::move(texture.m_basetype)),
                                                           m_format(std::move(texture.m_format)),
                                                           m_internalformat(std::move(texture.m_internalformat)),
                                                           m_datatype(std::move(texture.m_datatype))
{
    m_tmp_slot = 0;
}
TextureBase::~TextureBase()
{
    Deactivate();
    glActiveTexture(0);
    glDeleteTextures(1, &m_id);
}

void TextureBase::TexImage2D(GLenum target, GLint level, GLint border, const void *pixels)
{
    glTexImage2D(target, level, static_cast<GLint>(m_internalformat),
                 m_width, m_height, border, static_cast<GLenum>(m_format), static_cast<GLenum>(m_datatype), pixels);
}
void TextureBase::TexImage3D(GLenum target, GLint level, GLint border, int layers, const void *pixels)
{
    glTexImage3D(target, level, static_cast<GLint>(m_internalformat), m_width, m_height, layers, border, static_cast<GLenum>(m_format), static_cast<GLenum>(m_datatype), pixels);
}
void TextureBase::TexImage2DMultisample(GLenum target, GLsizei samples, GLboolean fixedsamplelocations)
{
    glTexImage2DMultisample(target, samples, static_cast<GLint>(m_internalformat),
                            m_width, m_height, fixedsamplelocations);
}

void TextureBase::Bind()
{
    glBindTexture(static_cast<GLenum>(m_basetype), m_id);
}
void TextureBase::Unbind()
{
    glBindTexture(static_cast<GLenum>(m_basetype), 0);
}

void TextureBase::Activate()
{
    m_tmp_slot = Texture::UseSlot();
    Texture::Active(m_tmp_slot);
    Bind();
}
void TextureBase::Deactivate()
{
    Unbind();
}

void TextureBase::SetParameter(gtype::TexParaType parameter, GLfloat value)
{
    glTexParameterf(static_cast<GLenum>(m_basetype), static_cast<GLenum>(parameter), value);
}
void TextureBase::SetParameter(gtype::TexParaType parameter, GLfloat *value)
{
    glTexParameterfv(static_cast<GLenum>(m_basetype), static_cast<GLenum>(parameter), value);
}

void TextureBase::Rescale(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    SettingTexture();
}
void TextureBase::GenerateMipmap()
{
    glGenerateTextureMipmap(m_id);
}
std::string_view TextureBase::GetTypeString()
{
    auto idx = static_cast<int>(m_type);
    if (idx == -1)
        return Texture::helper::type_2_string.back();
    return Texture::helper::type_2_string[idx];
}