#include "Engine/Core/GraphicsAPI.h"
namespace gapi
{
    void ClearBits(GLbitfield bits)
    {
        glClear(bits);
    }
    void ClearColor(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
    }

}