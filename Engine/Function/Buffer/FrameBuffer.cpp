#include "Engine/Function/Buffer/FrameBuffer.h"
using namespace NoobRenderer;
FrameBuffer::FrameBuffer()
    : BufferBase()
{
    InitBuffer();
}

FrameBuffer::~FrameBuffer()
{
    Unbind();
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::InitBuffer()
{
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::ToTexture(int attachment, int texturePrimitive, int textureID)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texturePrimitive, textureID, 0);
}

void FrameBuffer::BlitBufferFrom(FrameBuffer::Ptr &frameBuffer, unsigned int width, unsigned int height, GLbitfield mask, GLenum filter)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer->GetID());
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, mask, filter);
}

void FrameBuffer::SetRenderBuffer(unsigned int attachment, unsigned int renderBufferID)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBufferID);
}

void FrameBuffer::SetTexture2D(unsigned int attachment, unsigned int textureId, GLenum textarget, int level)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textarget, textureId, level);
}

void FrameBuffer::SetTexture3D(unsigned int attachment, unsigned int textureId, int level, int zoffset)
{
    glFramebufferTexture3D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_3D, textureId, level, zoffset);
}

void FrameBuffer::SetTexture(unsigned int attachment, unsigned int textureId, int level)
{
    glFramebufferTexture(GL_FRAMEBUFFER, attachment, textureId, level);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::IsComplete()
{
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}