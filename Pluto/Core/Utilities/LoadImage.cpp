#include "Core/Utilities/LoadImage.h"
#include "Core/FileSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "LoadImage.h"

uint8_t *pluto::utilities::LoadImageFromFile(const std::string &filename, uint16_t *width, uint16_t *height, uint32_t *bits, bool *isHDR)
{
    auto realPath = FileSystem::Instance().GetPhysicalPath(filename);
    const char *cfilename = realPath.c_str();

    int texWidth = 0, texHeight = 0, texChannels = 0;
    stbi_uc *pixels = nullptr;
    int sizeOfChannel = 8;
    if (stbi_is_hdr(cfilename))
    {
        sizeOfChannel = 32;
        pixels = (uint8_t *)stbi_loadf(cfilename, &texWidth, &texHeight, &texChannels, 0);

        if (isHDR)
            *isHDR = true;
    }
    else
    {
        pixels = stbi_load(cfilename, &texWidth, &texHeight, &texChannels, 0);

        if (isHDR)
            *isHDR = false;
    }

    // TODO support different texChannels
    if (texChannels != 4)
        texChannels = 4;

    if (width)
        *width = texWidth;
    if (height)
        *height = texHeight;
    if (bits)
        *bits = texChannels * sizeOfChannel; // texChannels;	  //32 bits for 4 bytes r g b a

    const uint64_t size = uint64_t(texWidth) * uint64_t(texHeight) * uint64_t(texChannels) * uint64_t(sizeOfChannel / 8U);
    uint8_t *result = new uint8_t[size];
    memcpy(result, pixels, size);

    stbi_image_free(pixels);
    return result;
}
