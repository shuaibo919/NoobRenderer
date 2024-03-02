#pragma once
#include "Engine/Core/GraphicsAPI.h"

#include <glm.hpp>
#include <memory>
#include <gtc/type_ptr.hpp>

namespace NoobRenderer
{
    class BufferBase
    {
        using Ptr = std::shared_ptr<BufferBase>;
    protected:
        GLuint id;
        virtual void InitBuffer() = 0;

    public:
        BufferBase() : id(0) {}
        virtual ~BufferBase() = default;

    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

    public:
        inline GLuint GetID() const { return id; }
    };
}