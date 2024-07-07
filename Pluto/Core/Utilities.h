#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
namespace pluto
{
    namespace utilities
    {
        static void HashCombine(uint64_t &seed) {}

        template <typename T, typename... Rest>
        static void GetMixedHash(uint64_t &seed, const T &v, Rest... rest)
        {
            std::hash<T> hasher;
            seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            GetMixedHash(seed, rest...);
        }

        std::string GetFilePathExtension(const std::string &FileName);
        std::string RemoveFilePathExtension(const std::string &FileName);
        std::string GetFileName(const std::string &FilePath);
        std::string GetFileLocation(const std::string &FilePath);
        std::string RemoveName(const std::string &FilePath);
        bool IsHiddenFile(const std::string &path);
        int32_t FindStringPosition(const std::string &string, const std::string &search, uint32_t offset);
        std::string StringRange(const std::string &string, uint32_t start, uint32_t length);
        std::string RemoveStringRange(const std::string &string, uint32_t start, uint32_t length);
        bool StringContains(const std::string &string, const std::string &chars);
        bool StartsWith(const std::string &string, const std::string &start);
        int32_t NextInt(const std::string &string);
        bool StringEquals(const std::string &string1, const std::string &string2);
        std::string StringReplace(std::string str, char ch1, char ch2);
        std::string StringReplace(std::string str, char ch);
        std::string &BackSlashesToSlashes(std::string &string);
        std::string &SlashesToBackSlashes(std::string &string);
        std::string BytesToString(uint64_t bytes);
    }
}
