#include "Engine/Function/Buffer/RenderBuffer.h"
using namespace NoobRenderer;

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height, int internalFormat)
    : BufferBase(), m_width(width), m_height(height), m_internalFormat(internalFormat)
{
    InitBuffer();
}

RenderBuffer::~RenderBuffer()
{
    Unbind();
    glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::InitBuffer()
{
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
}

void RenderBuffer::Rescale(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
}

void RenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::Unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

MultiSampleRenderBuffer::MultiSampleRenderBuffer(unsigned int width, unsigned int height, int internalFormat, int samples)
{
    m_samples = samples;
    m_width = width;
    m_height = height;
    m_internalFormat = internalFormat;
    InitBuffer();
}
void MultiSampleRenderBuffer::InitBuffer()
{
    glGenRenderbuffers(1, &id);
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_internalFormat, m_width, m_height);
}
void MultiSampleRenderBuffer::Rescale(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    glBindRenderbuffer(GL_RENDERBUFFER, id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_internalFormat, m_width, m_height);
}