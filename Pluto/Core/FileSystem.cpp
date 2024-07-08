#include "Core/FileSystem.h"
#include <algorithm>
using namespace pluto;

bool FileSystem::IsRelativePath(const char *path)
{
    if (!path || path[0] == '/' || path[0] == '\\')
    {
        return false;
    }

    if (strlen(path) >= 2 && isalpha(path[0]) && path[1] == ':')
    {
        return false;
    }

    return true;
}

bool FileSystem::IsAbsolutePath(const char *path)
{
    if (!path)
    {
        return false;
    }

    return !IsRelativePath(path);
}

std::string FileSystem::GetPhysicalPath(const std::string &path)
{
    if (path.substr(0, 8) == "//Assets")
    {
        return mAssetRootPath + path.substr(8, path.size());
    }
    return path;
}

const char *FileSystem::GetFileOpenModeString(FileOpenFlags flag)
{
    if (flag == FileOpenFlags::READ)
    {
        return "rb";
    }
    else if (flag == FileOpenFlags::WRITE)
    {
        return "wb";
    }
    else if (flag == FileOpenFlags::READ_WRITE)
    {
        return "rb+";
    }
    else if (flag == FileOpenFlags::WRITE_READ)
    {
        return "wb+";
    }
    else
    {
        return "rb";
    }
}

bool FileSystem::AbsolutePathToFileSystem(const std::string &path, std::string &outFileSystemPath, bool folder)
{
    std::string updatedPath = path;
    std::replace(updatedPath.begin(), updatedPath.end(), '\\', '/');

    if (updatedPath.find(mAssetRootPath) != std::string::npos)
    {
        std::string newPath = updatedPath;
        std::string newPartPath = "//Assets";
        newPath.replace(0, mAssetRootPath.size(), newPartPath);
        outFileSystemPath = newPath;
        return true;
    }

    outFileSystemPath = updatedPath;
    return false;
}

std::string FileSystem::AbsolutePathToFileSystem(const std::string &path, bool folder)
{
    std::string outPath;
    AbsolutePathToFileSystem(path, outPath, folder);
    return outPath;
}

#if defined(_WIN32) && defined(WIN32)
#include <Windows.h>
#include <wtypes.h>
#include <locale>
#include <fstream>

std::string WStringToString(const std::wstring &wstr)
{
    int count = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.length()), NULL, 0, NULL, NULL);
    std::string str(count, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], count, NULL, NULL);
    return str;
}

std::wstring StringToWString(const std::string &str)
{
    int count = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.length()), &wstr[0], count);
    return wstr;
}

/*void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
    {
    }*/

static HANDLE OpenFileForReading(const std::string &path)
{
    return CreateFileW(const_cast<LPCWSTR>(StringToWString(path).c_str()), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
}

static int64_t GetFileSizeInternal(const HANDLE file)
{
    LARGE_INTEGER size;
    GetFileSizeEx(file, &size);
    return size.QuadPart;
}

static bool ReadFileInternal(const HANDLE file, void *buffer, const int64_t size)
{
    OVERLAPPED ol = {0};
    return ReadFileEx(file, buffer, static_cast<DWORD>(size), &ol, nullptr) != 0;
}

bool FileSystem::FileExists(const std::string &path)
{
    DWORD dwAttrib = GetFileAttributesW(const_cast<LPCWSTR>(StringToWString(path).c_str()));
    return (dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool FileSystem::FolderExists(const std::string &path)
{
    DWORD dwAttrib = GetFileAttributesW(const_cast<LPCWSTR>(StringToWString(path).c_str()));
    return dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

int64_t FileSystem::GetFileSize(const std::string &path)
{
    const HANDLE file = OpenFileForReading(path);
    if (file == INVALID_HANDLE_VALUE)
        return -1;
    int64_t result = GetFileSizeInternal(file);
    CloseHandle(file);

    return result;
}

bool FileSystem::ReadFile(const std::string &path, void *buffer, int64_t size)
{
    std::ifstream stream(path, std::ios::binary | std::ios::ate);

    auto end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    size = end - stream.tellg();
    buffer = new char[size];
    stream.read((char *)buffer, size);
    stream.close();

    return buffer;
}

uint8_t *FileSystem::ReadFile(const std::string &path)
{
    if (!FileExists(path))
        return nullptr;

    std::ifstream stream(path, std::ios::binary | std::ios::ate);

    auto end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    const int64_t size = end - stream.tellg();
    char *buffer = new char[size];
    stream.read((char *)buffer, size);
    stream.close();

    return (uint8_t *)buffer;
}

std::string FileSystem::ReadTextFile(const std::string &path)
{
    if (!FileExists(path))
        return std::string();

    std::ifstream stream(path);

    std::string fileContent;
    std::string line;
    while (std::getline(stream, line))
    {                               // Read file line by line
        fileContent += line + "\n"; // Append each line to fileContent
    }

    stream.close();

    return fileContent;
}

bool FileSystem::WriteFile(const std::string &path, uint8_t *buffer, uint32_t size)
{
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);

    if (!stream)
    {
        stream.close();
        return false;
    }

    stream.write((char *)buffer, size);
    stream.close();

    return true;
}

bool FileSystem::WriteTextFile(const std::string &path, const std::string &text)
{
    return WriteFile(path, (uint8_t *)&text[0], (uint32_t)text.size());
}

#elif defined(__APPLE__)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fstream>
static bool ReadFileInternal(FILE *file, void *buffer, int64_t size, bool readbytemode)
{
    int64_t read_size;
    if (readbytemode)
        read_size = fread(buffer, sizeof(uint8_t), size, file);
    else
        read_size = fread(buffer, sizeof(char), size, file);

    if (size != read_size)
    {
        return false;
    }
    else
        return true;
}

bool FileSystem::FileExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool FileSystem::FolderExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

int64_t FileSystem::GetFileSize(const std::string &path)
{
    if (!FileExists(path))
        return -1;
    struct stat buffer;
    stat(path.c_str(), &buffer);
    return buffer.st_size;
}

bool FileSystem::ReadFile(const std::string &path, void *buffer, int64_t size)
{
    if (!FileExists(path))
        return false;
    if (size < 0)
        size = GetFileSize(path);
    buffer = new uint8_t[size + 1];
    FILE *file = fopen(path.c_str(), "r");
    bool result = false;
    if (file)
    {
        result = ReadFileInternal(file, buffer, size, true);
        fclose(file);
    }
    return result;
}

uint8_t *FileSystem::ReadFile(const std::string &path)
{
    if (!FileExists(path))
        return nullptr;
    int64_t size = GetFileSize(path);
    FILE *file = fopen(path.c_str(), "rb");
    uint8_t *buffer = new uint8_t[size];
    bool result = ReadFileInternal(file, buffer, size, true);
    fclose(file);
    if (!result && buffer)
        delete[] buffer;
    return result ? buffer : nullptr;
}

std::string FileSystem::ReadTextFile(const std::string &path)
{
    if (!FileExists(path))
        return std::string();
    int64_t size = GetFileSize(path);
    FILE *file = fopen(path.c_str(), "r");
    std::string result(size, 0);
    bool success = ReadFileInternal(file, &result[0], size, false);
    fclose(file);
    if (success)
    {
        // Strip carriage returns
        result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    }
    return success ? result : std::string();
}

bool FileSystem::WriteFile(const std::string &path, uint8_t *buffer, uint32_t size)
{
    FILE *file = fopen(path.c_str(), "wb");
    size_t output = fwrite(buffer, 1, size, file);
    fclose(file);
    return output > 0;
}

bool FileSystem::WriteTextFile(const std::string &path, const std::string &text)
{
    std::fstream filestr;
    filestr.open(text.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
    filestr.write(text.c_str(), text.size());
    filestr.close();
    return true;
}
#elif define(__linux__)

#endif
