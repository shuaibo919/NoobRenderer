#include "Core/Utilities.h"
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace pluto::utilities;

std::string GetFilePathExtension(const std::string &FileName)
{
    auto pos = FileName.find_last_of('.');
    if (pos != std::string::npos)
        return FileName.substr(pos + 1);
    return "";
}

std::string RemoveFilePathExtension(const std::string &FileName)
{
    auto pos = FileName.find_last_of('.');
    if (pos != std::string::npos)
        return FileName.substr(0, pos);
    return FileName;
}

std::string GetFileName(const std::string &FilePath)
{
    auto pos = FilePath.find_last_of('/');
    if (pos != std::string::npos)
        return FilePath.substr(pos + 1);

    pos = FilePath.find_last_of("\\");
    if (pos != std::string::npos)
        return FilePath.substr(pos + 1);

    return FilePath;
}

std::string GetFileLocation(const std::string &FilePath)
{
    auto pos = FilePath.find_last_of('/');
    if (pos != std::string::npos)
        return FilePath.substr(0, pos + 1);

    pos = FilePath.find_last_of("\\");
    if (pos != std::string::npos)
        return FilePath.substr(0, pos + 1);

    return FilePath;
}

std::string RemoveName(const std::string &FilePath)
{
    auto pos = FilePath.find_last_of('/');
    if (pos != std::string::npos)
        return FilePath.substr(0, pos + 1);

    pos = FilePath.find_last_of("\\");
    if (pos != std::string::npos)
        return FilePath.substr(0, pos + 1);

    return FilePath;
}

bool IsHiddenFile(const std::string &path)
{
    if (path != ".." && path != "." && path[0] == '.')
    {
        return true;
    }

    return false;
}

int32_t FindStringPosition(const std::string &string, const std::string &search, uint32_t offset)
{
    const char *str = string.c_str() + offset;
    const char *found = strstr(str, search.c_str());
    if (found == nullptr)
        return -1;
    return (int32_t)(found - str) + offset;
}

std::string StringRange(const std::string &string, uint32_t start, uint32_t length)
{
    return string.substr(start, length);
}

std::string RemoveStringRange(const std::string &string, uint32_t start, uint32_t length)
{
    std::string result = string;
    return result.erase(start, length);
}

bool StringContains(const std::string &string, const std::string &chars)
{
    return string.find(chars) != std::string::npos;
}

bool StartsWith(const std::string &string, const std::string &start)
{
    return string.find(start) == 0;
}

int32_t NextInt(const std::string &string)
{
    for (uint32_t i = 0; i < string.size(); i++)
    {
        if (isdigit(string[i]))
            return atoi(&string[i]);
    }
    return -1;
}

bool StringEquals(const std::string &string1, const std::string &string2)
{
    return strcmp(string1.c_str(), string2.c_str()) == 0;
}

std::string StringReplace(std::string str, char ch1, char ch2)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] == ch1)
            str[i] = ch2;
    }

    return str;
}

std::string StringReplace(std::string str, char ch)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] == ch)
        {
            str = std::string(str).substr(0, i) + std::string(str).substr(i + 1, str.length());
        }
    }

    return str;
}

std::string &BackSlashesToSlashes(std::string &string)
{
    size_t len = string.length();
    for (size_t i = 0; i < len; i++)
    {
        if (string[i] == '\\')
        {
            string[i] = '/';
        }
    }
    return string;
}

std::string &SlashesToBackSlashes(std::string &string)
{
    size_t len = string.length();
    for (size_t i = 0; i < len; i++)
    {
        if (string[i] == '/')
        {
            string[i] = '\\';
        }
    }
    return string;
}

std::string BytesToString(uint64_t bytes)
{
    static const float gb = 1024 * 1024 * 1024;
    static const float mb = 1024 * 1024;
    static const float kb = 1024;

    std::stringstream result;
    if (bytes > gb)
        result << std::fixed << std::setprecision(2) << (float)bytes / gb << " gb";
    else if (bytes > mb)
        result << std::fixed << std::setprecision(2) << (float)bytes / mb << " mb";
    else if (bytes > kb)
        result << std::fixed << std::setprecision(2) << (float)bytes / kb << " kb";
    else
        result << std::fixed << std::setprecision(2) << (float)bytes << " bytes";

    return result.str();
}
