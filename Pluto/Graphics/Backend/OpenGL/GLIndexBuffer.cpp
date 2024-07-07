#include "Graphics/Backend/OpenGL/GLIndexBuffer.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"

#include "Graphics/Backend/OpenGL/GL.h"

using namespace pluto::Graphics;

GLIndexBuffer::GLIndexBuffer(GLIndexBuffer::Properties *&&pProperties)
    : IndexBuffer(std::move(pProperties))
{
}

GLIndexBuffer::~GLIndexBuffer()
{
}

void GLIndexBuffer::Bind(CommandBuffer *commandBuffer) const
{
}

void GLIndexBuffer::Unbind() const
{
}

// GLIndexBuffer::GLIndexBuffer(uint16_t *data, uint32_t count, BufferUsage bufferUsage)
//     : m_Count(count), m_Usage(bufferUsage)
// {
//     GLCall(glGenBuffers(1, &m_Handle));
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
//     GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint16_t), data, BufferUsageToOpenGL(m_Usage)));
// }

// GLIndexBuffer::GLIndexBuffer(uint32_t *data, uint32_t count, BufferUsage bufferUsage)
//     : m_Count(count), m_Usage(bufferUsage)
// {
//     GLCall(glGenBuffers(1, &m_Handle));
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
//     GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, BufferUsageToOpenGL(m_Usage)));
// }

// GLIndexBuffer::~GLIndexBuffer()
// {
//     GLCall(glDeleteBuffers(1, &m_Handle));
// }

// void GLIndexBuffer::Bind(CommandBuffer *commandBuffer) const
// {
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
//     // GLRenderer::Instance()->GetBoundIndexBuffer() = m_Handle;
// }

// void GLIndexBuffer::Unbind() const
// {
//     GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//     // GLRenderer::Instance()->GetBoundIndexBuffer() = -1;
// }

// uint32_t GLIndexBuffer::GetCount() const
// {
//     return m_Count;
// }

// void *GLIndexBuffer::GetPointerInternal()
// {
//     void *result = nullptr;
//     if (!m_Mapped)
//     {
//         GLCall(result = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
//         m_Mapped = true;
//     }
//     else
//     {
//         // LUMOS_LOG_WARN("Index buffer already mapped");
//     }

//     return result;
// }

// void GLIndexBuffer::ReleasePointer()
// {
//     if (m_Mapped)
//     {
//         GLCall(glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
//         m_Mapped = false;
//     }
// }

// void GLIndexBuffer::Init()
// {
//     CreateFunc = CreateFuncGL;
//     Create16Func = CreateFunc16GL;
// }

// IndexBuffer *GLIndexBuffer::CreateFuncGL(uint32_t *data, uint32_t count, BufferUsage bufferUsage)
// {
//     return new GLIndexBuffer(data, count, bufferUsage);
// }

// IndexBuffer *GLIndexBuffer::CreateFunc16GL(uint16_t *data, uint32_t count, BufferUsage bufferUsage)
// {
//     return new GLIndexBuffer(data, count, bufferUsage);
// }
