#ifndef PLUTO_OPENGL_GL_BUFFER_OBJECT_H
#define PLUTO_OPENGL_GL_BUFFER_OBJECT_H

#include "OpenGL/GLHeaders.h"
#include <Backend/RHIBase.h>

namespace pluto::backend
{
    struct GLBufferObject : public RHIBufferObject
    {
        GLBufferObject(uint32_t size, BufferObjectBinding bindingType, BufferUsage usage) noexcept
            : RHIBufferObject(size), usage(usage), bindingType(bindingType)
        {
        }

        struct
        {
            GLuint id;
            union
            {
                GLenum binding;
                void *buffer;
            };
        } gl;
        BufferUsage usage;
        BufferObjectBinding bindingType;
        uint16_t age = 0;
    };
}

#endif