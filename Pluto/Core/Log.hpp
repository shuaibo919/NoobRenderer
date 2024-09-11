#ifndef PLUTO_ENGINE_LOG_HPP
#define PLUTO_ENGINE_LOG_HPP

#include "Core/Base.hpp"
#include <cstdio>
#include <cassert>

namespace pluto
{
    class Logger : public Singleton<Logger>
    {
    private:
        std::mutex mLoogerMutex;

    public:
        Logger() = delete;
        Logger(token) {};
        ~Logger() {};
        template <typename... Args>
        void log(const char *message_priority_str, const char *message, Args... args)
        {
            std::scoped_lock lock(mLoogerMutex);
            _logTime();
            _print("%s", message_priority_str);
            _print(message, args...);
            _print("\n");
        }

        void log(const char *message_priority_str, const char *message)
        {
            std::scoped_lock lock(mLoogerMutex);
            _logTime();
            _print("%s", message_priority_str);
            _print(message);
            _print("\n");
        }

    public:
        struct Info
        {
        };
        struct Warn
        {
        };
        struct Debug
        {
        };
        struct Error
        {
        };
        struct Critical
        {
        };

    private:
        Logger(const Logger &) = delete;
        Logger &operator=(const Logger &) = delete;
        void _logTime()
        {
            std::time_t current_time = std::time(0);
#if defined(_MSC_VER)
            tm timestamp;
            localtime_s(&timestamp, &current_time);
            char buffer[80];
            strftime(buffer, 80, "%c", &timestamp);
            printf("%s\t", buffer);
#else
            std::tm *timestamp = std::localtime(&current_time);
            char buffer[80];
            strftime(buffer, 80, "%c", timestamp);
            printf("%s\t", buffer);
#endif
        }
        template <typename... Args>
        void _print(const char *message, Args... args)
        {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
            printf(message, args...);
#pragma clang diagnostic pop
#elif defined(__GNUC__) || defined(__GNUG__)
            printf(message, args...);
#elif defined(_MSC_VER)
            printf_s(message, args...);
#else
            printf(message, args...);
#endif
        }
    };

    template <typename T>
    constexpr const static char *GetLevelTag()
    {
        if (std::is_same_v<T, pluto::Logger::Info>)
            return "[Info]\t";
        if (std::is_same_v<T, pluto::Logger::Warn>)
            return "[Warn]\t";
        if (std::is_same_v<T, pluto::Logger::Error>)
            return "[Error]\t";
        if (std::is_same_v<T, pluto::Logger::Debug>)
            return "[Debug]\t";
        if (std::is_same_v<T, pluto::Logger::Critical>)
            return "[Critical]\t";

        return "[Unknown]\t";
    }

    using Info = pluto::Logger::Info;
    using Warn = pluto::Logger::Warn;
    using Debug = pluto::Logger::Debug;
    using Error = pluto::Logger::Error;
    using Critical = pluto::Logger::Critical;

    template <typename T, typename... Args>
    static void log(const char *message, Args... args)
    {
        pluto::Logger::Instance().log(GetLevelTag<T>(), message, args...);
    }

    template <typename... Args>
    static void log(const char *message, Args... args)
    {
        pluto::Logger::Instance().log(GetLevelTag<Info>(), message, args...);
    }

    template <typename T>
    static void log(const char *message)
    {
        pluto::Logger::Instance().log(GetLevelTag<T>(), message);
    }

    static void log(const char *message)
    {
        pluto::Logger::Instance().log(GetLevelTag<Info>(), message);
    }

    constexpr static std::string __GET_LINE_INFO_STD_STR__(const char *file, const char *line)
    {
        return std::string("file") + file + " line:" + line;
    }

}

#define PLUTO_STRINGIZE(x) _PLUTO_STRINGIZE(x)
#define _PLUTO_STRINGIZE(x) #x
#define PLineInfo pluto::__GET_LINE_INFO_STD_STR__(__FILE__, PLUTO_STRINGIZE(__LINE__)).c_str()
#define PLog pluto::log
#define PInfo pluto::Info
#define PWarn pluto::Warn
#define PDebug pluto::Debug
#define PError pluto::Error
#define PCritical pluto::Critical
#define PAssert(exp, msg) assert(((void)msg, exp))

#endif