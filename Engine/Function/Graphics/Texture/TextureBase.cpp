#include "Engine/Function/Graphics/Texture/TextureBase.h"
using namespace NoobRenderer;

TextureBase::TextureBase(int width, int height, Texture::Type type, Texture::BaseType basetype,
                         Texture::Format format, Texture::Format internalformat, Texture::DataType datatype)
    : m_width(width), m_height(height), m_type(type), m_basetype(basetype),
      m_format(format), m_internalformat(internalformat), m_datatype(datatype)
{
    InitTextureUnits();
    glGenTextures(1, &m_id);
}
TextureBase::TextureBase(const TextureBase &texture)
    : m_id(texture.m_id), m_width(texture.m_width),
      m_height(texture.m_height), m_type(texture.m_type), m_basetype(texture.m_basetype),
      m_format(texture.m_format), m_internalformat(texture.m_internalformat),
      m_datatype(texture.m_datatype), m_params(texture.m_params)
{
    m_tmp_slot = 0;
}
TextureBase::TextureBase(TextureBase &&texture) noexcept : m_id(std::move(texture.m_id)), m_width(std::move(texture.m_width)),
                                                           m_height(std::move(texture.m_height)), m_type(std::move(texture.m_type)),
                                                           m_basetype(std::move(texture.m_basetype)),
                                                           m_format(std::move(texture.m_format)),
                                                           m_internalformat(std::move(texture.m_internalformat)),
                                                           m_datatype(std::move(texture.m_datatype)),
                                                           m_params(std::move(texture.m_params))

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
void TextureBase::TexStorage2D(GLenum target, GLsizei levels)
{
    glTextureStorage2D(target, levels, static_cast<GLint>(m_internalformat), m_width, m_height);
}
void TextureBase::TexStorage3D(GLenum target, GLsizei levels, GLsizei depth)
{
    glTexStorage3D(target, levels, static_cast<GLint>(m_internalformat), m_width, m_height, depth);
}
void TextureBase::Bind()
{
    glBindTexture(static_cast<GLenum>(m_basetype), m_id);
}
void TextureBase::BindImage(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
    glBindImageTexture(unit, m_id, level, layered, layer, access, format);
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
void TextureBase::ClearTexImage(GLint level, const void *pixels)
{
    glClearTexImage(m_id, level, static_cast<GLenum>(m_format), static_cast<GLenum>(m_datatype), pixels);
}
void TextureBase::SetParameter(GLenum pname, GLfloat value)
{
    glTextureParameterf(m_id, pname, value);
}
void TextureBase::SetParameter(GLenum pname, GLfloat *value)
{
    glTextureParameterfv(m_id, pname, value);
}
void TextureBase::SetParameter(GLenum pname, GLint value)
{
    glTextureParameteri(m_id, pname, value);
}
void TextureBase::SetParameter(GLenum pname, GLint *value)
{
    glTextureParameteriv(m_id, pname, value);
}
void TextureBase::SetParameterAndSave(GLenum pname, GLfloat value)
{
    m_params.paramf[pname] = value;
    glTextureParameterf(m_id, pname, value);
}
void TextureBase::SetParameterAndSave(GLenum pname, GLfloat *value)
{
    m_params.paramfv[pname] = value;
    glTextureParameterfv(m_id, pname, value);
}
void TextureBase::SetParameterAndSave(GLenum pname, GLint value)
{
    m_params.parami[pname] = value;
    glTextureParameteri(m_id, pname, value);
}
void TextureBase::SetParameterAndSave(GLenum pname, GLint *value)
{
    m_params.paramiv[pname] = value;
    glTextureParameteriv(m_id, pname, value);
}
void TextureBase::Rescale(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    SettingTexture();
    for (auto &[key, val] : m_params.paramf)
        glTextureParameterf(m_id, key, val);
    for (auto &[key, val] : m_params.paramfv)
        glTextureParameterfv(m_id, key, val);
    for (auto &[key, val] : m_params.parami)
        glTextureParameteri(m_id, key, val);
    for (auto &[key, val] : m_params.paramiv)
        glTextureParameteriv(m_id, key, val);
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