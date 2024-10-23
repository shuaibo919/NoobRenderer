#pragma once
#include <string>

namespace pluto
{
    namespace utilities
    {
        enum class ImageLoadingMode
        {
            Default = 0,
            Grey = 1,
            GrayAlpha = 2,
            RGB = 3,
            RGBAlpha = 4
        };
        
        struct ImageLoadingOptions
        {
            bool isHDR;
            std::string filename;
            ImageLoadingMode mode;
        };

        uint8_t *LoadImageFromFile(const std::string &filename, uint16_t *width, uint16_t *height, uint32_t *bits, bool *isHDR);
        uint8_t *LoadImageFromFile(const ImageLoadingOptions &options, uint16_t *width, uint16_t *height, uint32_t *bits);
    }
}