#include "Platform.h"
#include "Core/Log.hpp"
#include "Core/Utilities.h"

int main()
{
    std::string path = "Asset/Texture/wood_seamless_texture.png";
    uint16_t  texWidth = 0, texHeight = 0;
    uint32_t bits = 0;
    bool isHdr;


    auto data = pluto::utilities::LoadImageFromFile(path, &texWidth, &texHeight, &bits, &isHdr);
    if (data)
    {
        pluto::log<PInfo>("Load texture success, width: %d, height: %d, bits: %d", texWidth, texHeight, bits);
        delete [] data;
    }

    return 0;
}
