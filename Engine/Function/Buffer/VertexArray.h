#pragma once
#include "Engine/Function/Buffer/BufferBase.h"
namespace NoobRenderer
{
    /**
     * @brief 顶点VAO缓冲对象,构造完成后自动绑定
     */
    class VertexArray : public BufferBase
    {
    public:
        using Ptr = std::shared_ptr<VertexArray>;

        VertexArray();
        VertexArray(const VertexArray &vertexArray);
        VertexArray(VertexArray &&vertexArray) noexcept;
        VertexArray &operator=(const VertexArray &vertexArray);
        ~VertexArray();

    protected:
        void InitBuffer() override;

    public:
        void Bind() override;
        void Unbind() override;
    };
}