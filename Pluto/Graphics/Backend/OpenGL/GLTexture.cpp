#include "Graphics/Backend/OpenGL/GLTexture.h"
#include "Graphics/Backend/OpenGL/GLShader.h"
#include "Graphics/Backend/OpenGL/GL.h"
#include "GLTexture.h"
using namespace pluto::Graphics;

void *GLTexture2D::GetHandle() const
{
    return nullptr;
}

void GLTexture2D::Bind(uint32_t slot) const
{
}

void GLTexture2D::Unbind(uint32_t slot) const
{
}

GLTexture2D::~GLTexture2D()
{
}

GLTexture2D::GLTexture2D(Properties *&&pProperties)
    : Texture2D(std::move(pProperties))
{
}

void *GLTexture2DArray::GetHandle() const
{
    return nullptr;
}

void GLTexture2DArray::Bind(uint32_t slot) const
{
}

void GLTexture2DArray::Unbind(uint32_t slot) const
{
}

GLTexture2DArray::~GLTexture2DArray()
{
}

GLTexture2DArray::GLTexture2DArray(Properties *&&pProperties)
    : Texture2DArray(std::move(pProperties))
{
}

void *GLTextureCube::GetHandle() const
{
    return nullptr;
}

void GLTextureCube::Bind(uint32_t slot) const
{
}

void GLTextureCube::Unbind(uint32_t slot) const
{
}

GLTextureCube::~GLTextureCube()
{
}

GLTextureCube::GLTextureCube(Properties *&&pProperties)
    : TextureCube(std::move(pProperties))
{
}

void *GLTexture3D::GetHandle() const
{
    return nullptr;
}

void GLTexture3D::Bind(uint32_t slot) const
{
}

void GLTexture3D::Unbind(uint32_t slot) const
{
}

GLTexture3D::~GLTexture3D()
{
}

GLTexture3D::GLTexture3D(Properties *&&pProperties)
    : Texture3D(std::move(pProperties))
{
}
