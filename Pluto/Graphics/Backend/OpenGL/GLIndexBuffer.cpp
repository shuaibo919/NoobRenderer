/* OpenGL IndexBuffer */
#include "Graphics/Backend/OpenGL/GLIndexBuffer.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLCommandBuffer.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;

GLIndexBuffer::GLIndexBuffer(RenderContext *ctx, GLIndexBuffer::Properties *&&pProperties)
    : IndexBuffer(ctx, std::move(pProperties))
{
    GlCall(glGenBuffers(1, &mHandle));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, pProperties->count * pProperties->indicesType, pProperties->data, GLUtilities::GetBufferUsage(mProperties->usage)));
}

GLIndexBuffer::~GLIndexBuffer()
{
    GlCall(glDeleteBuffers(1, &mHandle));
}

void GLIndexBuffer::Bind(CommandBuffer *commandBuffer) const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle));
}

void GLIndexBuffer::Unbind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
