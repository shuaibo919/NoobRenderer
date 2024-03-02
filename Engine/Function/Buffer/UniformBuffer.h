#pragma once
#include "Engine/Function/Buffer/BufferBase.h"
#include <vector>

namespace NoobRenderer
{
    class UnifromBuffer : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<UnifromBuffer>;
        void SubData(GLintptr offset, GLsizeiptr size, const void *data);
        void Bind() override;
        void Unbind() override;

    private:
        size_t length;

    public:
        UnifromBuffer() = delete;
        UnifromBuffer(GLsizeiptr size, GLuint binding = 0);
        ~UnifromBuffer();
        inline GLuint GetBinding() const { return m_binding; }

    private:
        GLsizeiptr m_size;
        GLuint m_binding;
        void InitBuffer() override;
    };
}