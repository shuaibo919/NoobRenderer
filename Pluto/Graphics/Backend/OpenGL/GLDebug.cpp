#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Core/Log.hpp"

#ifdef PLUTO_DEBUG
bool pluto::Graphics::OpenGL::CheckError(const char *function, const char *file, const size_t line)
{
    GLenum err(glGetError());
    bool Error = true;
    while (err != GL_NO_ERROR)
    {
        std::string error;

        switch (err)
        {
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:;
        }
        pluto::log<pluto::Error>("GL_%s - %s - %s:%d", error.c_str(), file, function, line);
        Error = false;
        err = glGetError();
    }
    return Error;
}

#endif
