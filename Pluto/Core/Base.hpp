#ifndef PLUTO_ENGINE_BASE_HPP
#define PLUTO_ENGINE_BASE_HPP
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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Third
#include "nlohmann/json.hpp"

namespace pluto
{
    template <typename T>
    class Singleton
    {
    public:
        static T &Instance();

        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton) = delete;

    protected:
        struct token
        {
        };
        Singleton() {}
    };

    template <typename T>
    T &Singleton<T>::Instance()
    {
        static std::unique_ptr<T> _instance{new T{token{}}};
        return *_instance;
    }

    class Identity
    {
        template <typename Archive>
        friend void save(Archive &archive, const Identity &ID);

        template <typename Archive>
        friend void load(Archive &archive, Identity &ID);

    public:
        Identity() : m_identity(0){};
        Identity(uint64_t id) : m_identity(id) {}
        Identity(const Identity &other) : m_identity(other.m_identity) {}

        ~Identity() = default;
        operator uint64_t() { return m_identity; }
        operator const uint64_t() const { return m_identity; }

    private:
        uint64_t m_identity;
    };

    template <typename Properties, typename BuildClass>
    class BuildBase
    {
    public:
        BuildBase() : mProperties(new Properties()) {}
        ~BuildBase() = default;
        BuildBase(const BuildBase &) = delete;
        BuildBase &operator=(const BuildBase &) = delete;
        BuildBase(BuildBase &&) = delete;
        BuildBase &operator=(BuildBase &&) = delete;

    protected:
        Properties *mProperties;
    };

    template <typename T, typename F>
    static inline void update_state(T &state, T const &expected, F func) noexcept
    {
        if (state != expected) [[likely]]
        {
            state = expected;
            func();
        }
    }

    /// @brief 子系统 虚函数:Init(),Update()
    class SubSystem : public std::enable_shared_from_this<SubSystem>
    {
    public:
        SubSystem() = default;
        virtual ~SubSystem()
        {
        }

        virtual void Init() = 0;
        virtual void Update() = 0;
    };

}

#endif