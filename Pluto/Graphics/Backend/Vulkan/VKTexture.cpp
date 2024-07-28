/* Vulkan Texture */
#include "Graphics/Backend/Vulkan/VKTexture.h"
/* Common */
#include "Graphics/Backend/Vulkan/VKUtilities.h"
using namespace pluto::Graphics;

void *VKTexture2D::GetHandle() const
{
    return nullptr;
}

void VKTexture2D::Bind(uint32_t slot) const
{
}

void VKTexture2D::Unbind(uint32_t slot) const
{
}

VKTexture2D::~VKTexture2D()
{
}

VKTexture2D::VKTexture2D(RenderContext *ctx, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
}

VKTexture2D::VKTexture2D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2D(ctx, std::move(pProperties))
{
}

void *VKTexture2DArray::GetHandle() const
{
    return nullptr;
}

void VKTexture2DArray::Bind(uint32_t slot) const
{
}

void VKTexture2DArray::Unbind(uint32_t slot) const
{
}

VKTexture2DArray::~VKTexture2DArray()
{
}

VKTexture2DArray::VKTexture2DArray(RenderContext *ctx, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
}

VKTexture2DArray::VKTexture2DArray(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture2DArray(ctx, std::move(pProperties))
{
    // mProperties.
}

void *VKTextureCube::GetHandle() const
{
    return nullptr;
}

void VKTextureCube::Bind(uint32_t slot) const
{
}

void VKTextureCube::Unbind(uint32_t slot) const
{
}

VKTextureCube::~VKTextureCube()
{
}

VKTextureCube::VKTextureCube(RenderContext *ctx, Properties *&&pProperties)
    : TextureCube(ctx, std::move(pProperties))
{
}

VKTextureCube::VKTextureCube(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : VKTextureCube(ctx, std::move(pProperties))
{
    // mProperties.
}

void *VKTexture3D::GetHandle() const
{
    return nullptr;
}

void VKTexture3D::Bind(uint32_t slot) const
{
}

void VKTexture3D::Unbind(uint32_t slot) const
{
}

VKTexture3D::~VKTexture3D()
{
}

VKTexture3D::VKTexture3D(RenderContext *ctx, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}

VKTexture3D::VKTexture3D(RenderContext *ctx, const std::string &path, Properties *&&pProperties)
    : Texture3D(ctx, std::move(pProperties))
{
}
