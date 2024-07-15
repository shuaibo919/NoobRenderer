/* OpenGL VertexBuffer */
#include "Graphics/Backend/OpenGL/GLUniformBuffer.h"
/* Usage */
#include "Graphics/Backend/OpenGL/GLShader.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"

using namespace pluto::Graphics;
GLUniformBuffer::GLUniformBuffer(RenderContext *ctx, UniformBuffer::Properties *&&pProperties)
    : UniformBuffer(ctx, std::move(pProperties))
{
    GlCall(glGenBuffers(1, &mHandle));
    if (mProperties->size != UniformBuffer::Empty)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
        glBufferData(GL_UNIFORM_BUFFER, mProperties->size, mProperties->data, GL_DYNAMIC_DRAW);
    }
}

GLUniformBuffer::~GLUniformBuffer()
{
    GlCall(glDeleteBuffers(1, &mHandle));
}

void GLUniformBuffer::ReInit(uint32_t size, const void *data)
{
    mProperties->data = (uint8_t *)data;
    mProperties->size = size;
    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
    glBufferData(GL_UNIFORM_BUFFER, mProperties->size, mProperties->data, GL_DYNAMIC_DRAW);
}

void GLUniformBuffer::SetData(uint32_t size, const void *data)
{
    mProperties->data = (uint8_t *)data;
    GLvoid *p = nullptr;

    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);

    if (size != mProperties->size)
    {
        p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        mProperties->size = size;

        memcpy(p, mProperties->data, mProperties->size);
        glUnmapBuffer(GL_UNIFORM_BUFFER);
    }
    else
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, mProperties->size, mProperties->data);
    }
}

void GLUniformBuffer::SetDynamicData(uint32_t size, uint32_t typeSize, const void *data)
{
    mProperties->data = (uint8_t *)data;
    mProperties->size = size;
    mDynamic = true;
    mDynamicSize = typeSize;

    GLvoid *p = nullptr;

    glBindBuffer(GL_UNIFORM_BUFFER, mHandle);

    if (size != mProperties->size)
    {
        p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        mProperties->size = size;

        memcpy(p, mProperties->data, mProperties->size);
        glUnmapBuffer(GL_UNIFORM_BUFFER);
    }
    else
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, mProperties->size, mProperties->data);
    }
}

bool GLUniformBuffer::GetDynamic() const
{
    return mDynamic;
}

uint32_t GLUniformBuffer::GetSize() const
{
    return mProperties->size;
}

uint32_t GLUniformBuffer::GetHandle() const
{
    return mHandle;
}

uint32_t GLUniformBuffer::GetDynamicSize() const
{
    return mDynamicSize;
}