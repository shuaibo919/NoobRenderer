#pragma once
#include <string>
#include "Application/FontAwesome/IconsFontAwesome6.h"
namespace IconFont
{
    inline static std::string WithIcon(const char *iconFont, std::string &name)
    {
        return std::string(iconFont) + " " + name;
    }
    inline static std::string WithIcon(const char *iconFont, const std::string &name)
    {
        return std::string(iconFont) + " " + name;
    }
}