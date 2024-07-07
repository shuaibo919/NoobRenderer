#include "Graphics/Backend/OpenGL/GLFramebuffer.h"
// #include "Graphics/Backend/OpenGL/GLTexture.h"
// #include "Graphics/Backend/OpenGL/GLDebug.h"
// #include "Graphics/RHI/Texture.h"

// #include "Graphics/Backend/OpenGL/GL.h"

using namespace pluto::Graphics;
GLFramebuffer::GLFramebuffer(GLFramebuffer::Properties *&&pProperties)
    : Framebuffer(std::move(pProperties))
{
}

GLFramebuffer::~GLFramebuffer()
{
}

void GLFramebuffer::Validate()
{
}
void GLFramebuffer::SetClearColour(const glm::vec4 &colour)
{
}
