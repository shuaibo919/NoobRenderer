#include "Engine/Function/Buffer/VertexArray.h"
using namespace NoobRenderer;
VertexArray::VertexArray()
    : BufferBase()
{
    InitBuffer();
}

VertexArray::VertexArray(const VertexArray &vertexArray)
{
    id = vertexArray.id;
}

VertexArray::VertexArray(VertexArray &&vertexArray) noexcept
{
    id = vertexArray.id;
}

VertexArray &VertexArray::operator=(const VertexArray &vertexArray)
{
    id = vertexArray.id;
    return *this;
}

VertexArray::~VertexArray()
{
    Unbind();
    glDeleteVertexArrays(1, &id);
}
void VertexArray::InitBuffer()
{
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}

void VertexArray::Bind()
{
    glBindVertexArray(id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}