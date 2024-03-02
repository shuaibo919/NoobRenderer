#pragma once
#include "Engine/Function/Buffer/BufferBase.h"
#include <vector>

namespace NoobRenderer
{
    class IndexBuffer : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<IndexBuffer>;
        
        void Bind() override;
        void Unbind() override;
        void UpdateIndices(const std::vector<unsigned int> &indices);
        std::vector<unsigned int> GetIndices();

    private:
        size_t length;

    public:
        IndexBuffer();
        IndexBuffer(const std::vector<unsigned int> indices);
        IndexBuffer(const IndexBuffer &indexBuffer);
        IndexBuffer(IndexBuffer &&indexBuffer) noexcept;
        IndexBuffer &operator=(const IndexBuffer &indexBuffer);
        ~IndexBuffer();

    private:
        void InitBuffer(std::vector<unsigned int> indices);
        void InitBuffer() override;
    };
}