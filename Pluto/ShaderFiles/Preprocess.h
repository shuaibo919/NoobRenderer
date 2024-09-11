#pragma once
#include <mutex>
#include <ctime>
#include <chrono>
#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include "nlohmann/json.hpp"
namespace pluto
{
    using ShaderFiles = std::tuple<std::string, std::string, std::string, std::string>;
    static std::unordered_map<std::string, ShaderFiles> ShaderFilesMap;
    inline static void shader(const std::string &name, const std::string &vert, const std::string &frag, const std::string &geom, const std::string &comp)
    {
        ShaderFilesMap[name] = {vert, frag, geom, comp};
    }
} // namespace pluto
using namespace pluto;

#define ShaderListBegin       \
    namespace LoadShaderFiles \
    {                         \
        void Init()           \
        {

#define ShaderListEnd \
    }                 \
    }

#include "ShaderLists.txt"