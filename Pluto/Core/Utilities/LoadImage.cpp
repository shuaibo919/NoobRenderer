#include "Core/Utilities/LoadImage.h"
#include "Core/FileSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"

int ConvertToSTBIEnum(pluto::utilities::ImageLoadingMode mode)
{
    using namespace pluto::utilities;
    switch (mode)
    {
    case ImageLoadingMode::RGB:
        return STBI_rgb;

    case ImageLoadingMode::RGBAlpha:
        return STBI_rgb_alpha;

    case ImageLoadingMode::Grey:
        return STBI_grey;

    case ImageLoadingMode::GrayAlpha:
        return STBI_grey_alpha;
    default:
        break;
    }
    return STBI_default;
}

uint8_t *pluto::utilities::LoadImageFromFile(const std::string &filename, uint16_t *width, uint16_t *height, uint32_t *bits, bool *isHDR)
{
    auto sPhysicalPath = FileSystem::Instance().GetPhysicalPath(filename);

    int texWidth = 0, texHeight = 0, texChannels = 0;
    stbi_uc *pixels = nullptr;
    int sizeOfChannel = 8;
    if (stbi_is_hdr(sPhysicalPath.c_str()))
    {
        sizeOfChannel = 32;
        pixels = (uint8_t *)stbi_loadf(sPhysicalPath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (isHDR)
            *isHDR = true;
    }
    else
    {
        pixels = stbi_load(sPhysicalPath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

        if (isHDR)
            *isHDR = false;
    }

    if (pixels == nullptr)
    {
        return nullptr;
    }

    if (texChannels != 4)
    {
        texChannels = 4;
    }

    if (width)
        *width = texWidth;
    if (height)
        *height = texHeight;
    if (bits)
        *bits = texChannels * sizeOfChannel; // texChannels;	  //32 bits for 4 bytes r g b a

    const size_t size = size_t(texWidth) * size_t(texHeight) * size_t(texChannels) * size_t(sizeOfChannel / 8U);
    uint8_t *result = new uint8_t[size];
    memcpy(result, pixels, size);

    stbi_image_free(pixels);

    return result;
}

uint8_t *pluto::utilities::LoadImageFromFile(const ImageLoadingOptions &options, uint16_t *width, uint16_t *height, uint32_t *bits)
{
    if (options.flipY)
    {
        stbi_set_flip_vertically_on_load(true);
    }
    auto sPhysicalPath = FileSystem::Instance().GetPhysicalPath(options.filename);
    int texWidth = 0, texHeight = 0, texChannels = 0;
    stbi_uc *pixels = nullptr;
    int sizeOfChannel = options.isHDR ? 32 : 8;
    int reqComp = ConvertToSTBIEnum(options.mode);
    if (options.isHDR)
    {
        pixels = (uint8_t *)stbi_loadf(sPhysicalPath.c_str(), &texWidth, &texHeight, &texChannels, reqComp);
    }
    else
    {
        pixels = stbi_load(sPhysicalPath.c_str(), &texWidth, &texHeight, &texChannels, reqComp);
    }

    if (pixels == nullptr)
    {
        return nullptr;
    }

    if (options.flipY)
    {
        stbi_set_flip_vertically_on_load(false);
    }

    if (width)
        *width = texWidth;
    if (height)
        *height = texHeight;
    if (bits)
        *bits = texChannels * sizeOfChannel; // texChannels;	  //32 bits for 4 bytes r g b a

    const size_t size = size_t(texWidth) * size_t(texHeight) * size_t(texChannels) * size_t(sizeOfChannel / 8U);
    uint8_t *result = new uint8_t[size];
    memcpy(result, pixels, size);

    stbi_image_free(pixels);

    return result;
}