#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "Engine/Function/Buffer/BufferBase.h"
#include "Engine/Function/Buffer/IndexBuffer.h"
#include "Engine/Function/Buffer/Vertex.h"

namespace NoobRenderer
{
    class VertexBuffer : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<VertexBuffer>;

        VertexBuffer();
        VertexBuffer(size_t _length);
        VertexBuffer(std::vector<Vertex> &vertices);
        VertexBuffer(std::vector<Vertex> &vertices, std::vector<unsigned int> indices);
        VertexBuffer(const VertexBuffer &vertexBuffer);
        VertexBuffer(VertexBuffer &&vertexBuffer) noexcept;
        VertexBuffer &operator=(const VertexBuffer &vertexBuffer);
        ~VertexBuffer();

    private:
        IndexBuffer::Ptr indexBuffer;
        bool hasIndexBuffer;
        size_t length;

    protected:
        void VertexAttributes();
        void InitBuffer(std::vector<Vertex> &vertices, std::vector<unsigned int> indices);
        void InitBuffer() override;

    public:
        // void UpdateVertices(std::vector<Vertex> &vertices);
        // void UpdateVertex(int pos, Vertex newVertex);
        std::vector<Vertex> GetVertices();
        void Bind() override;
        void Unbind() override;

    public:
        inline IndexBuffer::Ptr &GetIndexBuffer() { return indexBuffer; }
        inline bool HasIndexBuffer() const { return hasIndexBuffer; }
        inline void SetLength(size_t length) { this->length = length; }
        inline size_t GetLength() const { return length; }
    };
}