#include "Engine/Function/Buffer/UniformBuffer.h"
using namespace NoobRenderer;
UnifromBuffer::UnifromBuffer(GLsizeiptr size, GLuint binding)
    : BufferBase(), m_size(size), m_binding(binding)
{
    InitBuffer();
}

UnifromBuffer::~UnifromBuffer()
{
    Unbind();
    glDeleteBuffers(1, &id);
}

void UnifromBuffer::InitBuffer()
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_UNIFORM_BUFFER, id);
    glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_binding, id);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UnifromBuffer::SubData(GLintptr offset, GLsizeiptr size, const void *data)
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UnifromBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, id);
}

void UnifromBuffer::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
