#ifndef PLUTO_OPENGL_GL_TEXTURE_H
#define PLUTO_OPENGL_GL_TEXTURE_H

#include "OpenGL/GLHeaders.h"
#include <Backend/RHIBase.h>

namespace pluto::backend
{
    struct GLTexture : public RHITexture
    {
        struct GL
        {
            GL() noexcept : imported(false), sidecarSamples(1) {}
            GLuint id = 0;
            GLenum target = 0;
            GLenum internalFormat = 0;
            GLuint sidecarRenderBufferMS = 0;
            GLfloat anisotropy = 1.0;
            int8_t baseLevel = 127;
            int8_t maxLevel = -1;
            bool imported;
            uint8_t sidecarSamples;

        } gl;
    };
}

#endif