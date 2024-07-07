#pragma once
#include "Core/Base.hpp"
namespace pluto
{
    enum class FileOpenFlags
    {
        READ,
        WRITE,
        READ_WRITE,
        WRITE_READ
    };

    class FileSystem : public Singleton<FileSystem>
    {
    public:
        FileSystem() = delete;
        FileSystem(token){};
        ~FileSystem(){};

    public:
        bool AbsolutePathToFileSystem(const std::string &path, std::string &outFileSystemPath, bool folder = false);
        std::string AbsolutePathToFileSystem(const std::string &path, bool folder = false);
        void SetAssetRoot(std::string root) { mAssetRootPath = root; };

    private:
        std::string mAssetRootPath;

    public:
        static bool FileExists(const std::string &path);
        static bool FolderExists(const std::string &path);
        static int64_t GetFileSize(const std::string &path);

        static uint8_t *ReadFile(const std::string &path);
        static bool ReadFile(const std::string &path, void *buffer, int64_t size = -1);
        static std::string ReadTextFile(const std::string &path);

        static bool WriteFile(const std::string &path, uint8_t *buffer, uint32_t size);
        static bool WriteTextFile(const std::string &path, const std::string &text);

        // static std::string GetWorkingDirectory();

        static bool IsRelativePath(const char *path);
        static bool IsAbsolutePath(const char *path);
        static const char *GetFileOpenModeString(FileOpenFlags flag);
    };

}