#include "Engine/Function/Buffer/IndexBuffer.h"
using namespace NoobRenderer;

IndexBuffer::IndexBuffer() : BufferBase() { }

IndexBuffer::IndexBuffer(const std::vector<unsigned int> indices)
    : BufferBase(), length(indices.size()) {
    InitBuffer(indices);
}

IndexBuffer::IndexBuffer(const IndexBuffer& indexBuffer) 
    : length(indexBuffer.length) {
    id = indexBuffer.id;
}

IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept 
    : length(indexBuffer.length) {
    id = indexBuffer.id;
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& indexBuffer) {
    id = indexBuffer.id;
    length = indexBuffer.length;
    return *this;
}

IndexBuffer::~IndexBuffer() {
    Unbind();
    glDeleteBuffers(1, &id);
}

void IndexBuffer::InitBuffer(std::vector<unsigned int> indices) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
}

void IndexBuffer::InitBuffer() { }

void IndexBuffer::Bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::UpdateIndices(const std::vector<unsigned int>& indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    unsigned int* ptr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &indices[0], indices.size());
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<unsigned int> IndexBuffer::GetIndices() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    unsigned int* ptr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

    std::vector<unsigned int> indices;
    for(int i = 0; i < length; i ++) indices.push_back(ptr[i]);
    
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return indices;
}