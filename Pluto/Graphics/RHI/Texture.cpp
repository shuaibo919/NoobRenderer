#include "Graphics/RHI/Texture.h"
#include "Graphics/RHI/GraphicsContext.h"
using namespace pluto::Graphics;

Texture::Builder &Texture::Builder::SetBase(uint16_t width, uint16_t height, uint16_t samples, RHIFormat format, uint16_t zAxisSize)
{
    mProperties->samples = samples;
    mProperties->format = format;
    mProperties->width = width;
    mProperties->height = height;
    mProperties->zAxisSize = zAxisSize;
    return *this;
}

Texture::Builder &Texture::Builder::SetFilter(TextureFilter minFilter, TextureFilter magFilter)
{
    mProperties->minFilter = minFilter;
    mProperties->magFilter = magFilter;
    return *this;
}

Texture::Builder &Texture::Builder::SetWrap(TextureWrap wrap)
{
    mProperties->wrap = wrap;
    return *this;
}

Texture::Builder &Texture::Builder::SetAdancedOptions(TextureFlags flag, bool srgb, bool mipmap, bool anisotropic)
{
    mProperties->flags = flag;
    mProperties->srgb = srgb;
    mProperties->generateMipMaps = mipmap;
    mProperties->anisotropicFiltering = anisotropic;
    return *this;
}

Texture::Ptr Texture::Builder::Create(Texture::Type type, const SharedPtr<GraphicsContext> &pContext)
{
    mProperties->type = type;
    return pContext->CreateTexture(static_cast<uint16_t>(type), std::move(mProperties));
}

Texture::Ptr Texture::Builder::Create(Texture::Properties &desc, const SharedPtr<GraphicsContext> &pContext)
{
    *mProperties = desc;
    return pContext->CreateTexture(static_cast<uint16_t>(desc.type), std::move(mProperties));
}

Texture::Ptr Texture::Builder::Create(Texture::Type type, const std::string &path, const SharedPtr<GraphicsContext> &pContext)
{
    mProperties->type = type;
    return pContext->CreateTexture(static_cast<uint16_t>(type), path, std::move(mProperties));
}

Texture::Ptr Texture::Builder::Create(const std::string &path, Texture::Properties &desc, const SharedPtr<GraphicsContext> &pContext)
{
    *mProperties = desc;
    return pContext->CreateTexture(static_cast<uint16_t>(desc.type), path, std::move(mProperties));
}

Texture::Texture(RenderContext *ctx, Properties *&&pProperties)
    : mProperties(pProperties), RHIBase(ctx)
{
}
Texture::~Texture()
{
    if (mProperties != nullptr)
        delete mProperties;
}

Texture2D::Texture2D(RenderContext *ctx, Properties *&&pProperties)
    : Texture(ctx, std::move(pProperties))
{
}

Texture2D::~Texture2D() = default;

Texture2DArray::Texture2DArray(RenderContext *ctx, Properties *&&pProperties)
    : Texture(ctx, std::move(pProperties))
{
}

Texture2DArray::~Texture2DArray() = default;

TextureCube::TextureCube(RenderContext *ctx, Properties *&&pProperties)
    : Texture(ctx, std::move(pProperties))
{
}

TextureCube::~TextureCube() = default;

Texture3D::Texture3D(RenderContext *ctx, Properties *&&pProperties)
    : Texture(ctx, std::move(pProperties))
{
}

Texture3D::~Texture3D() = default;