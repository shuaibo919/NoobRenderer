#pragma once
namespace pluto
{
    namespace Graphics
    {
        namespace OpenGL
        {
#ifdef PLUTO_DEBUG
            bool CheckError(const char *function, const char *file, const int line);
#define GlCall(x)                       \
    while (glGetError() != GL_NO_ERROR) \
        ;                               \
    x;                                  \
    pluto::Graphics::OpenGL::CheckError(#x, __FILE__, __LINE__);
#else
#define GlCall(x) x
#endif
        }
    }
}
