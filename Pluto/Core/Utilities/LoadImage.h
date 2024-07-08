#pragma once
#include <string>

namespace pluto
{
    namespace utilities
    {
        uint8_t *LoadImageFromFile(const std::string &filename, uint16_t *width, uint16_t *height, uint32_t *bits, bool *isHDR);
    }
}