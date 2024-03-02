﻿#include "Engine/Function/Buffer/FrameBuffer.h"
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

void FrameBuffer::BlitFrom(FrameBuffer::Ptr &frameBuffer, unsigned int width, unsigned int height)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer->GetID());
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::SetRenderBuffer(unsigned int attachment, unsigned int renderBufferID)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBufferID);
}

void FrameBuffer::SetTexture2D(unsigned int attachment, unsigned int textureId, GLenum textarget, int level)
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textarget, textureId, level);
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