#include "Engine/Function/Graphics/Texture/WriteToTexture.h"
using namespace NoobRenderer;
WriteToTexture::WriteToTexture(unsigned int width, unsigned int height) : m_width(width), m_height(height)
{
    m_fbo = std::make_shared<FrameBuffer>();
    m_fbo->Bind();
}
WriteToTexture::~WriteToTexture()
{
    m_fbo.reset();
    if (m_rbo != nullptr)
        m_rbo.reset();
    m_texs.clear();
}
void WriteToTexture::ExplicitColorAttach()
{
    if (tex_attachments.size() > 0)
    {
        glNamedFramebufferDrawBuffers(m_fbo->GetID(), static_cast<GLsizei>(tex_attachments.size()), tex_attachments.data());
    }
}
void WriteToTexture::DiscardRenderBuffer()
{
    glNamedFramebufferDrawBuffer(m_fbo->GetID(), GL_NONE);
    glNamedFramebufferReadBuffer(m_fbo->GetID(), GL_NONE);
}
void WriteToTexture::BlitBufferFrom(const WriteToTexture::Ptr &write2tex, GLbitfield mask, GLenum filter)
{
    auto fbo = write2tex->m_fbo;
    m_fbo->BlitBufferFrom(fbo, m_width, m_height, mask, filter);
}
void WriteToTexture::Rescale(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_fbo->Bind();
    if (m_rbo != nullptr)
    {
        m_rbo->Rescale(width, height);
        m_fbo->SetRenderBuffer(rbo_attachment, m_rbo->GetID());
        m_rbo->Unbind();
    }
    for (int i = 0; i < tex_attachments.size(); i++)
    {
        m_texs[i]->Rescale(width, height);
        m_fbo->SetTexture2D(tex_attachments[i], m_texs[i]->GetID());
        m_texs[i]->Unbind();
    }
    m_fbo->Unbind();
}
TextureBase::Ptr &WriteToTexture::GetTexture(int idx)
{
    return m_texs[idx];
}
void WriteToTexture::ActivateTex(int idx)
{
    m_texs[idx]->Activate();
}
void WriteToTexture::DeactivateTex(int idx)
{
    m_texs[idx]->Deactivate();
}
int WriteToTexture::GetTexSlot(int idx)
{
    return m_texs[idx]->GetTempSlot();
}
GLint WriteToTexture::GetTexId(int idx)
{
    return m_texs[idx]->GetID();
}
size_t WriteToTexture::GetTextureSize()
{
    return m_texs.size();
}