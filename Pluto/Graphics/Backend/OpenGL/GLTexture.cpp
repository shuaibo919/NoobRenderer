/* OpenGL Texture */
#include "Graphics/Backend/OpenGL/GLTexture.h"
/* Common */
#include "Graphics/Backend/OpenGL/GL.h"
#include "Graphics/Backend/OpenGL/GLDebug.h"
#include "Graphics/Backend/OpenGL/GLUtilities.h"
using namespace pluto::Graphics;

void *GLTexture2D::GetHandle() const
{
    return (void *)(size_t)mHandle;
}

void GLTexture2D::Bind(uint32_t slot) const
{
    GlCall(glActiveTexture(GL_TEXTURE0 + slot));
    GlCall(glBindTexture(GL_TEXTURE_2D, mHandle));
}

void GLTexture2D::Unbind(uint32_t slot) const
{
    GlCall(glActiveTexture(GL_TEXTURE0 + slot));
    GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

GLTexture2D::~GLTexture2D()
{
    GlCall(glDeleteTextures(1, &mHandle));
}

GLTexture2D::GLTexture2D(RenderContext *ctx, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
    mHandle = LoadTexture(nullptr);
}

GLTexture2D::GLTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
    uint8_t *data = LoadTextureData(path);
    mHandle = LoadTexture(data);
    delete[] data;
}

uint8_t *GLTexture2D::LoadTextureData(const std::string &path)
{
    uint8_t *pixels = nullptr;
    mProperties->width = 0;
    mProperties->height = 0;
    uint32_t bits;
    bool isHDR;
    pixels = utilities::LoadImageFromFile(path.c_str(), &(mProperties->width), &(mProperties->height), &bits, &isHDR);
    mProperties->format = Utilities::BitsToFormat(bits);

    return pixels;
};
uint32_t GLTexture2D::LoadTexture(uint8_t *data)
{
    GLint minFilter = mProperties->minFilter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;
    if (GL_LINEAR && (mProperties->generateMipMaps || mProperties->flags & TextureFlags::TextureCreateMips))
        minFilter = GL_LINEAR_MIPMAP_LINEAR;
    GLint magFilter = mProperties->magFilter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST;
    GLint wraps = GLUtilities::GetTextureWrap(mProperties->wrap);
    GLint wrapt = GLUtilities::GetTextureWrap(mProperties->wrap);
    GLint Iformat = GLUtilities::GetTextureFormatSpecific(mProperties->format, mProperties->srgb);
    GLenum format = GLUtilities::GetTextureFormat(Iformat);
    GLenum type = GLUtilities::GetDataType(mProperties->format);
    uint32_t handle;
    GlCall(glGenTextures(1, &handle));
    GlCall(glBindTexture(GL_TEXTURE_2D, handle));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps));
    GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt));

    if (mProperties->anisotropicFiltering)
    {
        // Todo
    }

    GlCall(glTexImage2D(GL_TEXTURE_2D, 0, Iformat, mProperties->width, mProperties->height, 0, format, type, data));
    if (mProperties->generateMipMaps || mProperties->flags & TextureFlags::TextureCreateMips)
    {
        GlCall(glGenerateMipmap(GL_TEXTURE_2D));
    }

    return handle;
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

GLTexture2DArray::GLTexture2DArray(RenderContext *ctx, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
}

GLTexture2DArray::GLTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
    // mProperties.
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

GLTextureCube::GLTextureCube(RenderContext *ctx, Properties *&&pProperties)
    : TextureCube(ctx, std::move(pProperties))
{
}

GLTextureCube::GLTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : GLTextureCube(ctx, std::move(pProperties))
{
    // mProperties.
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

GLTexture3D::GLTexture3D(RenderContext *ctx, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}

GLTexture3D::GLTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}
